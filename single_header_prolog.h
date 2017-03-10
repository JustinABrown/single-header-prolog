// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.

// VERSION 0.0.1

#ifndef SINGLE_HEADER_PROLOG_H
#define SINGLE_HEADER_PROLOG_H


#ifndef PROGRAM_SIZE
#define PROGRAM_SIZE 300
#endif


namespace single_header_prolog {


  enum class ResultType
  {
    SUCCESS, FAILED_TO_MATCH, NOT_FOUND, UNKNOWN, END_REACHED
  };

  constexpr int primes[9] = { 23, 19, 17, 13, 11, 7, 5, 3, 2 }; // 32 bit!

  
  template <class T> class Term;
  template <class T> class Atom;
  template <class T> class Variable;
  template <class T> class Cut;
  template <class T> class Fail;
  template <class T> class Clause;
  template <class T> class Program;

    
  template <class T>
  struct History {
    int force_skip = 0; // Skip unification of variables manually
    int skipSelections = 1;
    int currentGoal = 0;
    int shiftedGoal = 0;    
    Variable<T>* previousChainstart = nullptr;    

    History() {}
    
    void skip_vars(int num)
    {
      for (int i = 0; i < num; i++) {
	if (((num >> i) & 0x01) == 1) {
	  skipSelections *= primes[i];
	}
      }
    }

    void unbind()
    {
      if (previousChainstart != nullptr) {
	previousChainstart->unbind();
      }
    }

    void reset()
    {
      if (previousChainstart != nullptr) {
	previousChainstart->reset();
      }
    }    
  };




  template <class T> 
  bool match(Term<T>*, Term<T>*, History<T>*);



  
  template <class T> 
  class Term
  {
  private:
    struct Term2
    {
      int arity = 0;
      Term<T>* subterms[32] = {nullptr};

      Term2()
      {}
      
      Term2(int)
      {}

      template <typename Current, typename... Args>
      Term2(int i, Current cur, Args... rest)
	: Term2(++i, rest...)
      {
	subterms[i] = cur;
	arity++;
      }
    };

  protected:
    const T functor;
    
  private:
    Term2 term2;

  public:
    explicit Term(const T& n)
      : functor(n)
    {}

    template <typename... Args>
    explicit Term(const T& n, Args... args)
      : functor(n),
	term2(0, args...)
    {}

    virtual int get_arity() const;

    virtual Term<T>* get_term(int) const;
    
    virtual bool unify(Term<T>*, History<T>&);
    
    virtual const Term<T>* get_binding() const;

    virtual void assign(Term<T>*) {} 

    virtual Variable<T>* get_chainstart();

    virtual void set_isPartOfHead();
    
    virtual T get_functor() const; // todo '&'?

    virtual void lock_binding() {}
    
    virtual void reset() {}
    
    virtual ~Term() noexcept = default;
  };


  
  template <class T>   
  class Atom : public Term<T>
  {
    using Term<T>::functor;
    
  public:
    explicit Atom(const T& n)
      : Term<T>(n)
    {}

    int get_arity() const override;

    Term<T>* get_term(int) const override;

    bool unify(Term<T>*, History<T>&) override;

    Variable<T>* get_chainstart() override;
  }; 



  template <class T> 
  class Variable : public Term<T>
  {
    using Term<T>::functor;
    
  private:
    void appendToChain(Variable<T>* link)
    {
      varId++;
      origId = varId; // Restore .varId in unbind()
      
      if (next != nullptr) {
	next->appendToChain(link);
      }
      else {
	next = link;
      }
    }
    
    Term<T>* binding = nullptr;

    int varId = 0;
    int origId = 0;

    int chainLen;
    
    Variable<T>* next;
    Variable<T>* first;    

    bool isHead = false;
    bool locked = false;
    
  public:
    explicit Variable(const T& n)
      : Term<T>(n), chainLen(1), next(nullptr)
    {
      first = this;
    }
    
    explicit Variable(const T& n, Variable<T>& fst)
      : Term<T>(n), chainLen(1), next(nullptr), first(&fst)
    {
      fst.appendToChain(this);
      fst.chainLen++;
    }

    int get_arity() const override;

    Term<T>* get_term(int) const override;

    bool unify(Term<T>*, History<T>&) override;
    
    const Term<T>* get_binding() const override;

    void assign(Term<T>*) override;

    Variable<T>* get_chainstart() override;

    int get_chainlen() const;

    void unbind();

    void lock_binding() override;
    
    void reset() override;
  };



  template <class T>
  class Cut : public Term<T>
  {
  public:
    explicit Cut(const T& n)
      : Term<T>(n)
    {}

    int get_arity() const override;

    bool unify(Term<T>*, History<T>&) override;
  };



  template <class T>
  class Fail : public Term<T>
  {
  public:
    explicit Fail(const T& n)
      : Term<T>(n)
    {}

    int get_arity() const override;

    bool unify(Term<T>*, History<T>&) override;
  };


  
  template <class T>   
  class Clause : public Term<T>
  {
  private:
    struct Goals
    {
      int goalCnt = 0;
      Term<T>* goals[32] = {nullptr};

      Goals()
      {}

      Goals(int)
      {}

      template <typename Current, typename... Args>
      Goals(int i, Current cur, Args... rest)
	: Goals(++i, rest...)
      {
	goals[i] = cur;
	goalCnt++;
      }
    };
    
    Term<T>* head;
    Goals goals;
    
  public:
    explicit Clause(Term<T>* h, const T& neck)
      : Term<T>(neck),
        head(h)
    {
      head->set_isPartOfHead();
    }

    template <typename... Args>
    explicit Clause(Term<T>* h, const T& neck, Args... args)
      : Term<T>(neck),
	head(h),
	goals(0, args...)
    {
      head->set_isPartOfHead();
    }
    
    int get_arity() const override;

    Term<T>* get_term(int) const override;
    
    bool unify(Term<T>*, History<T>&) override;

    Term<T>* get_head() const;

    int get_goalCnt() const;

    Term<T>* get_goal(int) const;

    Variable<T>* get_chainstart() override;
    
    T get_functor() const override;//todo &!
  };
  


  template <class T> 
  class Program
  {
  private:
    struct Program2
    {
      int size = 0;
      Clause<T>* clauses[PROGRAM_SIZE] = {nullptr};

      Program2(int)
      {}

      template <typename Current, typename... Args>
      Program2(int i, Current cur, Args... rest)
	: Program2(++i, rest...)
      {
	clauses[i] = cur;
	size++;
      }
    };

    Program2 program2;

  public:
    template <typename... Args>
    Program(Args... args)
      : program2(0, args...)
    {}

    Clause<T>* find(const T&, const int) const;

    int size() const;
  };




  template <class T> 
  int Term<T>::get_arity() const
  {
    return term2.arity;
  }

  template <class T> 
  Term<T>* Term<T>::get_term(int pos) const
  {
    return term2.subterms[pos];
  }

  template <class T> 
  bool Term<T>::unify(Term<T>* other, History<T>&)   
  {
    return functor == other->functor;
  }

  template <class T> 
  const Term<T>* Term<T>::get_binding() const
  {
    return this;
  }

  template <class T> 
  Variable<T>* Term<T>::get_chainstart() 
  {
    for (int currentTerm = 1; currentTerm <= term2.arity; currentTerm++) {
      Variable<T>* chainStart = term2.subterms[currentTerm]->get_chainstart();
      if (chainStart != nullptr) {
	return chainStart;
      }
    }
    return nullptr;
  }

  template <class T>
  void Term<T>::set_isPartOfHead()
  {
    this->lock_binding();
    
    for (int currentTerm = 1; currentTerm <= term2.arity; currentTerm++) {
      term2.subterms[currentTerm]->set_isPartOfHead();
    }
  }

  template <class T> 
  T Term<T>::get_functor() const
  {
    return functor;
  }



  template <class T> 
  int Atom<T>::get_arity() const
  {
    return 1;
  }

  template <class T> 
  Term<T>* Atom<T>::get_term(int) const
  {
    return nullptr;
  }

  template <class T> 
  bool Atom<T>::unify(Term<T>* other, History<T>&)  
  {
    return other->get_functor() == functor;
  }

  template <class T> 
  Variable<T>* Atom<T>::get_chainstart()
  {
    return nullptr;
  }



  template <class T> 
  int Variable<T>::get_arity() const
  {
    return 1;
  }

  template <class T> 
  Term<T>* Variable<T>::get_term(int) const 
  {
    return nullptr;
  }

  template <class T> 
  bool Variable<T>::unify(Term<T>* other, History<T>& history) 
  {
    other->assign(binding);

    if (binding == nullptr) {
      if (history.skipSelections % primes[varId] == 0) { 
	history.skipSelections = history.skipSelections / primes[varId]; 
	varId++; // We might be dealing w/ the same variable again
	return false; 
      }
      binding = other;
      if (isHead) {
	locked = true;
      }
      return true;
    }
    else if (get_binding() == other->get_binding()) {
      return true;
    }
    else if (binding->get_functor() == other->get_functor()) {
      return match(binding, other, history); // This allows a variable to be called directly from the body of a clause
    }
    else {
      return false; // Couldn't unify
    }
  }

  template <class T> 
  const Term<T>* Variable<T>::get_binding() const
  {
    return binding == nullptr ? nullptr : binding->get_binding();
  }

  template <class T>
  void Variable<T>::assign(Term<T>* other)
  {
    if (binding == nullptr) {
      binding = other;
      if (isHead) {
	locked = true;
      }
    }
  }

  template <class T> 
  Variable<T>* Variable<T>::get_chainstart()
  {
    return first;
  }

  template <class T> 
  int Variable<T>::get_chainlen() const
  {
    return chainLen;
  }

  template <class T> 
  void Variable<T>::unbind()
  {
    varId = origId;
    
    if (!locked) {
      binding = nullptr;
    }
    
    if (next != nullptr) {
      next->unbind();
    }
  }

  template <class T>
  void Variable<T>::lock_binding()
  {
    isHead = true;
  }

  template <class T>
  void Variable<T>::reset()
  {
    varId = origId;
    
    binding = nullptr;
    locked = false;

    if (next != nullptr) {
      next->reset();
    }
  }
       


  template <class T>
  int Cut<T>::get_arity() const
  {
    return 2;
  }

  template <class T>
  bool Cut<T>::unify(Term<T>*, History<T>& history)
  {
    history.shiftedGoal = history.currentGoal + 1;
    return true;
  }

  

  template <class T>
  int Fail<T>::get_arity() const
  {
    return 2;
  }

  template <class T>
  bool Fail<T>::unify(Term<T>*, History<T>&)
  {
    return false;
  }


  
  template <class T> 
  int Clause<T>::get_arity() const
  {
    return goals.goalCnt == 0 ? head->get_arity() : 2;
  }

  template <class T> 
  Term<T>* Clause<T>::get_term(int pos) const
  {
    if (pos == 1) {
      return head;
    }
    else {                         // This is only used for unification. If you
      return goals.goals[pos - 1]; // want to match a goal use get_goal() instead!
    }                              //
  }

  template <class T> 
  bool Clause<T>::unify(Term<T>* other, History<T>&) 
  {
    bool isIdenticalHead = true;//match(other->get_term(1), head);
    bool isIdenticalBody = true;//match(other->get_term(2), goals.goals[1]);//todo all goals! update: not if all goals inside 1 term!
    // todo during loopng ^^ check if either one is nullptr, if so return false. if BOTH nupltr at the same time: true!
    return (isIdenticalHead && isIdenticalBody);
  }

  template <class T> 
  Term<T>* Clause<T>::get_head() const
  {
    return head;
  }

  template <class T> 
  int Clause<T>::get_goalCnt() const
  {
    return goals.goalCnt;
  }

  template <class T> 
  Term<T>* Clause<T>::get_goal(int pos) const
  {
    return goals.goals[pos];
  }

  template <class T> 
  Variable<T>* Clause<T>::get_chainstart() 
  {
    for (int currentGoal = 1; currentGoal <= goals.goalCnt; currentGoal++) {
      Variable<T>* chainStart = goals.goals[currentGoal]->get_chainstart();
      if (chainStart != nullptr) {
	return chainStart;
      }
    }
    return nullptr;
  }
  
  template <class T> 
  T Clause<T>::get_functor() const
  {
    return head->get_functor();
  }



  template <class T> 
  Clause<T>* Program<T>::find(const T& name, const int offset) const
  {
    int pos = 0;
    
    for (int i = 1; i <= program2.size; i++) {      
      if (program2.clauses[i]->get_functor() == name) {
	if (pos == offset) {
	  return program2.clauses[i];
	}
	else {
	  pos++;
	}
      }
    }					      

    return nullptr; // Predicate not found
  }

  template <class T> 
  int Program<T>::size() const
  {
    return program2.size;
  }






  template <class T> 
  bool match(Term<T>* lhs, Term<T>* rhs, History<T>& history)
  {
    if (lhs == nullptr || rhs == nullptr) {
      return true; // Assume success
    }

    const int lhs_arity = lhs->get_arity();
    const int rhs_arity = rhs->get_arity();

    
    if (lhs_arity != rhs_arity) { 
      return false;
    }

    for (int pos = 1; pos <= lhs_arity; pos++) {
      if (!rhs->unify(lhs, history) && !lhs->unify(rhs, history)) {
	return false;
      }
      else if (!match(lhs->get_term(pos), rhs->get_term(pos), history)) {
	return false;
      }
    }

    return true;
  }


 

  template <class T> 
  ResultType solve(const Program<T>& program, const int offset, Term<T>& goal, History<T>& history)
  {
    Clause<T>* predicate = program.find(goal.get_binding()->get_functor(), offset);

    if (predicate == nullptr && offset > 0) {
      return ResultType::END_REACHED;
    }
    else if (predicate == nullptr) {
      return ResultType::NOT_FOUND; 
    }

    if (match(predicate->get_head(), &goal, history)) {      
      if (predicate->get_goal(1) == nullptr) { // Empty body means success
      	return ResultType::SUCCESS;            //
      }                                        //


      
      // Body is not empty, check if all goals can be met:
      
      Variable<T>* chainStart = predicate->get_chainstart();
      history.previousChainstart = chainStart;

      int goalCnt = predicate->get_goalCnt();

      History<T> localHistory;
      localHistory.skip_vars(history.force_skip);
      localHistory.force_skip = history.force_skip;

      int backtrackCntr = 1;

      ResultType result = ResultType::UNKNOWN;


      
      for (int currentGoal = 1; currentGoal <= goalCnt; currentGoal++) { // Solve each goal in the body. Conjunction is implied
	localHistory.currentGoal = currentGoal; // Only needed for the ! operator
	int position = 0;
  	result = ResultType::UNKNOWN;

	while (result != ResultType::END_REACHED) {
	  result = solve(program, position, *predicate->get_goal(currentGoal), localHistory);
	
	  if (result == ResultType::NOT_FOUND) {
	    return result;
	  }
	  else if (result == ResultType::SUCCESS) {
	    break;
	  }

	  position++;
	}
 

	// Backtracking starts here:
	
	if (result == ResultType::END_REACHED) { 
	  if (backtrackCntr > program.size()) { // No more possibilities. Fail
	    if (chainStart != nullptr) {
	      chainStart->reset();
	    }
	    return ResultType::FAILED_TO_MATCH;
	  }
	  else {
	    if (chainStart != nullptr) {
	      chainStart->unbind();
	    }

	    localHistory.skipSelections = 1;
	    localHistory.skip_vars(history.force_skip);	    
	    localHistory.skip_vars(backtrackCntr);

	    backtrackCntr++;
	    currentGoal = localHistory.shiftedGoal; // Reset currentGoal to restart the for-loop. .shiftedGoal is offset by the ! operator
	  }
	}
      }
   

      return result;
    }
    else {
      return ResultType::FAILED_TO_MATCH;
    }
  }

  
} // namespace single_header_prolog

#endif // SINGLE_HEADER_PROLOG_H

