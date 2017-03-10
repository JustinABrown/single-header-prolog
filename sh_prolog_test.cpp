#include "single_header_prolog.h"
#include <iostream>
#include <string>
#include <chrono>
#include <array>
#include <random>
#include <functional>
#include <unordered_set>
#include <unordered_map>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

typedef single_header_prolog::Atom<std::string> ATOM;
typedef single_header_prolog::Term<std::string> TERM;
typedef single_header_prolog::Clause<std::string> CLAUSE;
typedef single_header_prolog::Variable<std::string> VARIABLE;
typedef single_header_prolog::Program<std::string> PROGRAM;


TEST_CASE("basic")
{
  using namespace single_header_prolog;
  using namespace std;

  ATOM fact01Atom("a");
  TERM fact01("g", &fact01Atom);
  TERM fact1b("f", &fact01);
  CLAUSE fact1c(&fact1b, ":-"); // <- into knowledge base

  ATOM fact02Atom("b");
  TERM fact02("f", &fact02Atom);
  CLAUSE fact2a(&fact02, ":-"); // <- into knowledge base
  
  ATOM fact03Atom1("i");
  ATOM fact03Atom2("j");  
  TERM fact03("blah", &fact03Atom1, &fact03Atom2);
  CLAUSE fact3a(&fact03, ":-"); // <- into knowledge base
  
  ATOM fact04Atom1("fact4_1");
  TERM fact4("h", &fact04Atom1);
  CLAUSE fact4a(&fact4, ":-"); // <- into knowledge base
  ATOM fact04Atom2("fact4_2");
  TERM fact4b("h", &fact04Atom2);
  TERM fact4d("h", &fact4b);
  CLAUSE fact4c(&fact4d, ":-"); // <- into knowledge base
 
  ATOM fact05Atom1("fact5_1");
  ATOM fact05Atom2("fact5_2");
  TERM fact5a("c", &fact05Atom1, &fact05Atom2);
  ATOM fact05Atom3("fact5_2");
  ATOM fact05Atom4("fact5_3");
  TERM fact5b("d", &fact05Atom3, &fact05Atom4);
  TERM fact05d("e", &fact5a, &fact5b);
  CLAUSE fact05c(&fact05d, ":-");  // <- into knowledge base
  
  ATOM xxx("no");
  TERM xx2("replyYes", &xxx);
  CLAUSE xx3(&xx2, ":-"); // <- into knowledge base
  ATOM yyy("yes");
  TERM yy2("replyYes", &yyy);
  CLAUSE yy3(&yy2, ":-"); // <- into knowledge base
  ATOM socrates("socrates");
  TERM man1("man", &socrates);
  CLAUSE manA(&man1, ":-"); // <- into knowledge base
  ATOM plato("plato");
  TERM man2("man", &plato);  
  CLAUSE manB(&man2, ":-"); // <- into knowledge base
  ATOM yes("yes");
  TERM aliveBody1("replyYes", &yes);
  VARIABLE Xxx("X_checkThis");
  TERM aliveHead("alive", &Xxx);
  TERM aliveBody2("man", &Xxx);
  CLAUSE alive(&aliveHead, ":-", &aliveBody1, &aliveBody2); // <- into knowledge base

  VARIABLE AAA1("AAA");
  TERM nfy("nfy", &AAA1, &AAA1);
  CLAUSE clnfy(&nfy, ":-"); // <- into knowledge base
  VARIABLE XXX("XXX");
  VARIABLE YYY("YYY", XXX);
  VARIABLE ZZZ("ZZZ", XXX);
  TERM thead("t", &XXX, &YYY, &ZZZ);
  TERM tbody2("nfy", &XXX, &YYY);
  TERM tbody3("nfy", &XXX, &ZZZ);
  CLAUSE t(&thead, ":-", &tbody2, &tbody3); // <- into knowledge base

  ATOM tFa("a");
  TERM tF1("m", &tFa);
  CLAUSE m1(&tF1, ":-");  // <- into knowledge base
  ATOM tFb("b");
  TERM tF2("m", &tFb);
  CLAUSE m2(&tF2, ":-");  // <- into knowledge base
  ATOM tGa("a");
  TERM tG1("n", &tGa);
  CLAUSE n1(&tG1, ":-");  // <- into knowledge base
  ATOM tGb("b");
  TERM tG2("n", &tGb);
  CLAUSE n2(&tG2, ":-");  // <- into knowledge base
  ATOM tHb("b");
  TERM tH1("o", &tHb);
  CLAUSE o1(&tH1, ":-");  // <- into knowledge base
  VARIABLE X("X");
  TERM kHead("k", &X);
  TERM mBody("m", &X);
  TERM nBody("n", &X);
  TERM oBody("o", &X);
  CLAUSE test888(&kHead, ":-", &mBody, &nBody, &oBody);

  ATOM   vincent("vincent");
  ATOM       mia("mia");
  TERM    loves1("loves", &vincent, &mia);
  CLAUSE cLoves1(&loves1, ":-");
  ATOM marcellus("marcellus");
  TERM    loves2("loves", &marcellus, &mia);
  CLAUSE cLoves2(&loves2, ":-");
  //  ATOM rodrigues("rodrigues");
  //  TERM    loves5("loves", &rodrigues, &mia);
  //  CLAUSE cLoves5(":-", &loves5);  
  VARIABLE A("A");
  VARIABLE B("B", A);
  VARIABLE C("C", A);
  TERM   jealous("jealous", &A, &B);
  TERM    loves3("loves", &A, &C);
  TERM    loves4("loves", &B, &C);
  CLAUSE test999(&jealous, ":-", &loves3, &loves4);
  
  VARIABLE Xa("Xa");
  ATOM man3("man3");
  TERM pred1("pred", &Xa, &Xa, &man3);
  CLAUSE testaaa(&pred1, ":-");

  
  PROGRAM knowledge_base(&fact1c, &fact2a, &fact3a, &fact4a, &fact4c, &fact05c,
			 &manA, &manB, &xx3, &yy3, &alive, &clnfy, &t,
			 &m1, &m2, &n1, &n2, &o1, &test888,
			 &cLoves1, &cLoves2, &test999,
			 &testaaa);

  
  ResultType success;


  SUBCASE("Test 1a + 1b") {
    VARIABLE X01a("X01a");
    TERM query1("f", &X01a);
    History<std::string> history;
    success = solve(knowledge_base, 0, query1, history);
    if (success == ResultType::SUCCESS) cout << "Test 1a, should be 'g' -> f(g(a)).        " << X01a.get_functor() << " = " << X01a.get_binding()->get_functor() << endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 1a not found" << endl;
    else cout << "Test 1a fail" << endl;
    REQUIRE(success == ResultType::SUCCESS);
    REQUIRE(X01a.get_binding()->get_functor() == "g");
  
    VARIABLE X01b("X01b");
    TERM query2("f", &X01b);
    success = solve(knowledge_base, 1, query2, history);
    if (success == ResultType::SUCCESS) cout << "Test 1b, should be 'b' -> f(b).           " << X01b.get_functor() << " = " << X01b.get_binding()->get_functor() << endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 1b not found" << endl;
    else cout << "Test 1b fail" << endl;
    REQUIRE(success == ResultType::SUCCESS);
    REQUIRE(X01b.get_binding()->get_functor() == "b");
  }


  SUBCASE("Test 2a + 2b") {
    VARIABLE Y02a("Y02a");
    TERM q2A("v", &Y02a);
    TERM q2B("f", &q2A);
    History<std::string> history;   
    success = solve(knowledge_base, 0, q2B, history);
    cout << "Test 2a, should be '1' (false/fail) " << (int)success << endl;
    REQUIRE(success == ResultType::FAILED_TO_MATCH);
  
    VARIABLE Y02b("Y02b");
    TERM q2C("g", &Y02b);
    TERM q2D("f", &q2C);
    success = solve(knowledge_base, 0, q2D, history);
    if (success == ResultType::SUCCESS) cout << "Test 2b, should be 'a' -> f(g(a)).        " << Y02b.get_functor() << " = " << Y02b.get_binding()->get_functor() << endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 2b not found" << endl;
    else cout << "Test 2b fail" << endl;
    REQUIRE(success == ResultType::SUCCESS);
    REQUIRE(Y02b.get_binding()->get_functor() == "a");
  }


  SUBCASE("Test 3a") {
    ATOM a03("i");
    VARIABLE X03("X03");
    TERM t03("blah", &a03, &X03);
    History<std::string> history;    
    success = solve(knowledge_base, 0, t03, history);
    if (success == ResultType::SUCCESS) cout << "Test 3a, should be 'j' -> fact03(i, j).   " << X03.get_functor() << " = " << X03.get_binding()->get_functor() << endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 3a not found" << endl;
    else cout << "Test 3a fail" << endl;
    REQUIRE(X03.get_binding()->get_functor() == "j");
    REQUIRE(success == ResultType::SUCCESS);
  }


  SUBCASE("Test 4a + 4b + 4c") {
    VARIABLE X04a("X04a");
    TERM q4A("h", &X04a);
    History<std::string> history;    
    success = solve(knowledge_base, 0, q4A, history);
    if (success == ResultType::SUCCESS) cout << "Test 4a, should be 'fact4_1' -> h(fact4_1).       " << X04a.get_functor() << " = " << X04a.get_binding()->get_functor() << endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 4a not found" << endl;
    else cout << "Test 4a fail" << endl;
    REQUIRE(X04a.get_binding()->get_functor() == "fact4_1");
    VARIABLE X04b("X04b");
    TERM q4B("h", &X04b);
    success = solve(knowledge_base, 1, q4B, history);
    if (success == ResultType::SUCCESS) cout << "Test 4b, should be 'h(fact4_2)' -> h(h(fact4_2)). " << X04b.get_functor() << " = " << X04b.get_binding()->get_functor() << endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 4b not found" << endl;
    else cout << "Test 4b fail" << endl;
    REQUIRE(X04b.get_binding()->get_functor() == "h");
    REQUIRE(success == ResultType::SUCCESS);
    VARIABLE X04c("X04c");
    TERM q4C("h", &X04c);
    success = solve(knowledge_base, 2, q4C, history);
    if (success == ResultType::END_REACHED) cout << "Test 4c, should be '4' (end_reached) " << (int)success << endl;
    else cout << "Test 4c, should not be 'fact4_2' -> h(h(fact4_2)). " << X04c.get_functor() << " = " << X04c.get_binding()->get_functor() << endl;
    REQUIRE(success == ResultType::END_REACHED);
  }


  SUBCASE("Test 5a") {
    ATOM a05a("fact5_1");  
    VARIABLE X05("X05");
    TERM q5A("c", &a05a, &X05);
    ATOM a05c("fact5_3");
    TERM q5B("d", &X05, &a05c);
    TERM q5("e", &q5A, &q5B);
    History<std::string> history;    
    success = solve(knowledge_base, 0, q5, history);
    if (success == ResultType::SUCCESS) cout << "Test 5a, should be 'fact5_2' " << X05.get_functor() << " = " << X05.get_binding()->get_functor() << endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 5a not found" << endl;
    else cout << "Test 5a fail" << endl;
    REQUIRE(X05.get_binding()->get_functor() == "fact5_2");
    REQUIRE(success == ResultType::SUCCESS);
  }


  SUBCASE("Test 6a alive(socrates, plato, ...)") {
    VARIABLE X06a("X06a");
    TERM a06a("alive", &X06a);
    History<std::string> history;    
    success = solve(knowledge_base, 0, a06a, history);
    /*    cout << "success: " << (int)success << endl;// todo
	  VARIABLE X06b("X06b");
	  TERM a06b("alive", &X06b);
	  success = solve(knowledge_base, 0, a06b, path);
	  path.pr();*/
    if (success == ResultType::SUCCESS) cout << "Test 6a, should be 'socrates' " << X06a.get_functor() << " = "
					     << X06a.get_binding()->get_functor() << endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 6a not found" << endl;
    else cout << "Test 6a fail" << endl;
    CHECK(X06a.get_binding()->get_functor() == "socrates");

    history.reset();
    history.force_skip = 1;
    

    VARIABLE X06b("X06b");
    TERM a06b("alive", &X06b);
    success = solve(knowledge_base, 0, a06b, history);
    if (success == ResultType::SUCCESS) cout << "Test 6b, should be 'plato' " << X06a.get_functor() << " = "
    					     << X06a.get_binding()->get_functor() << endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 6b not found" << endl;
    else cout << "Test 6b fail" << endl;
    CHECK(X06a.get_binding()->get_functor() == "plato");
  /* TODO:
man(socrates).
man(plato).

replyYes(no).
replyYes(yes).

animal(dog).

alive(X) :- replyYes(yes), man(X).
alive(X) :- animal(X).

Query:
  alive(Z).

Result:
  Z = socrates
  Z = plato
  Z = dog // <- into knowledge base
  */
  }


  SUBCASE("Test 7a") {
    ATOM chicken("chicken");
    TERM a07a("t", &chicken, &chicken, &chicken);
    History<std::string> history;        
    success = solve(knowledge_base, 0, a07a, history);
    if (success == ResultType::SUCCESS) cout << "Test 7a, should succeed. Ok." << endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 7a not found" << endl;
    else cout << "Test 7a fail" << endl;
    REQUIRE(success == ResultType::SUCCESS);
  }


  SUBCASE("Test 8a (backtracking)") {
    VARIABLE Y("Y");
    TERM a08a("k", &Y);
    History<std::string> history;
    history.force_skip += 1;
    success = solve(knowledge_base, 0, a08a, history);
    if (success == ResultType::SUCCESS) cout << "Test 8a, should be 'b' -> 'h(b)." << Y.get_functor() << " = "
					     << Y.get_binding()->get_functor() << endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 8a not found" << endl;
    else cout << "Test 8a fail" << endl;
    CHECK(Y.get_binding()->get_functor() == "b");
    CHECK(success == ResultType::SUCCESS);
  }


  SUBCASE("Test 9a 'jealous'") {
    VARIABLE JX("JX");
    VARIABLE JY("JY");
    TERM q9("jealous", &JX, &JY);
    History<std::string> history;
    success = solve(knowledge_base, 0, q9, history);
    if (success == ResultType::SUCCESS) cout << "Test 9a, should be 'vincent' -> 'loves(vincent, mia). " << JX.get_functor() << " = "
					     << JX.get_binding()->get_functor() << "\n"
					     << "and 'vincent' -> 'loves(vincent, mia). " << JY.get_functor() << " = "
					     << JY.get_binding()->get_functor() <<endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 9a not found" << endl;
    else cout << "Test 9a fail" << endl;
    CHECK(JX.get_binding()->get_functor() == "vincent");
    CHECK(JY.get_binding()->get_functor() == "vincent");

    history.reset();
    //int totalCombinations = 1;
    //    history.combinations_ptr = &totalCombinations;
    history.force_skip = 4; // Should get "marcellus" and "vincent"
    
    VARIABLE KX("KX");
    VARIABLE KY("KY");
    TERM q9b("jealous", &KX, &KY);

    success = solve(knowledge_base, 0, q9b, history);
    if (success == ResultType::SUCCESS) cout << "Test 9b, should be 'marcellus'" << KX.get_functor() << " = "
					     << KX.get_binding()->get_functor() << "\n"
					     << "and 'vincent'" << KY.get_functor() << " = "
					     << KY.get_binding()->get_functor() <<endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 9b not found" << endl;
    else cout << "Test 9b fail" << endl;
    CHECK(KX.get_binding()->get_functor() == "marcellus");    
    CHECK(KY.get_binding()->get_functor() == "vincent");
  }


  SUBCASE("Test 10a 'london'") {
    ATOM london("london");
    ATOM dog("dog");
    VARIABLE Aa("Aa");
    TERM qa("pred", &london, &dog, &Aa);
    History<std::string> history;    
    success = solve(knowledge_base, 0, qa, history);
    cout << "Test 10a, should be '1' (false/fail) " << (int)success << endl;
    REQUIRE(success == ResultType::FAILED_TO_MATCH);

    
    
    VARIABLE Ab("Ab");
    TERM qb("pred", &london, &london, &Ab);
    success = solve(knowledge_base, 0, qb, history);
    if (success == ResultType::SUCCESS) cout << "Test 10b, should be 'man3.' " << Ab.get_functor() << " = "
					     << Ab.get_binding()->get_functor() << endl;
    else if (success == ResultType::NOT_FOUND) cout << "Test 10b not found" << endl;
    else cout << "Test 10b fail" << endl;
    REQUIRE(Ab.get_binding()->get_functor() == "man3");
    REQUIRE(success == ResultType::SUCCESS);
  }
}


TEST_CASE("mapcolor")
{
  using namespace single_header_prolog;
  using namespace std;

  chrono::time_point<chrono::system_clock> start, end;
  start = chrono::system_clock::now();          
  
  /*
+-----+---------+
|     | b       |
|  a  +------+  |
|     |   c  |  |
|     +------+--+
|     |         |
+-----+   d     |
|  e  |         |
+-----+---------+
  */
  ATOM regionA("reg_a");
  ATOM regionB("reg_b");
  ATOM regionC("reg_c");
  ATOM regionD("reg_d");
  ATOM regionE("reg_e");  
  TERM adj01("adjacent", &regionA, &regionB);       
  CLAUSE cadj01(&adj01, ":-");
  TERM adj02("adjacent", &regionA, &regionC);
  CLAUSE cadj02(&adj02, ":-");  
  TERM adj03("adjacent", &regionA, &regionD);
  CLAUSE cadj03(&adj03, ":-");  
  TERM adj04("adjacent", &regionA, &regionE);
  CLAUSE cadj04(&adj04, ":-");
  TERM adj05("adjacent", &regionB, &regionC);
  CLAUSE cadj05(&adj05, ":-");
  TERM adj06("adjacent", &regionB, &regionD);
  CLAUSE cadj06(&adj06, ":-");
  TERM adj07("adjacent", &regionC, &regionD);
  CLAUSE cadj07(&adj07, ":-");
  TERM adj08("adjacent", &regionD, &regionE);
  CLAUSE cadj08(&adj08, ":-");
  TERM adj09("adjacent", &regionB, &regionA);
  CLAUSE cadj09(&adj09, ":-");
  TERM adj10("adjacent", &regionC, &regionA);
  CLAUSE cadj10(&adj10, ":-");
  TERM adj11("adjacent", &regionD, &regionA);
  CLAUSE cadj11(&adj11, ":-");
  TERM adj12("adjacent", &regionE, &regionA);
  CLAUSE cadj12(&adj12, ":-");
  TERM adj13("adjacent", &regionC, &regionB);
  CLAUSE cadj13(&adj13, ":-");
  TERM adj14("adjacent", &regionD, &regionB);
  CLAUSE cadj14(&adj14, ":-");
  TERM adj15("adjacent", &regionD, &regionC);
  CLAUSE cadj15(&adj15, ":-");
  TERM adj16("adjacent", &regionE, &regionD);
  CLAUSE cadj16(&adj16, ":-");
  ATOM mapcolorHead("conflict");
  VARIABLE MapX("MapX");
  VARIABLE MapY("MapY");
  TERM adjacent("adjacent", &MapX, &MapY);
  VARIABLE Color("Color");
  TERM color1("color", &MapX, &Color);
  TERM color2("color", &MapY, &Color);
  CLAUSE conflict1(&mapcolorHead, ":-", &adjacent, &color1, &color2);  
  CLAUSE conflict2(&mapcolorHead, ":-", &color1, &adjacent, &color2);
  CLAUSE conflict3(&mapcolorHead, ":-", &color2, &color1, &adjacent);
  CLAUSE conflict4(&mapcolorHead, ":-", &color1, &adjacent, &color2);  
  array<ATOM, 5> colorAtoms { ATOM("\e[1;31mred\e[0m"), ATOM("\e[1;32mgreen\e[0m"), ATOM("\e[1;34mblue\e[0m"), ATOM("\e[1;33myellow\e[0m"), ATOM("\e[1;35mpink\e[0m") };

  ResultType success;
    
  int sel0=0, sel1=1, sel2=2, sel3=3, sel4=3; // default vals for debugging. Last '3' repeats!

  static mt19937 mt;
  mt.seed(random_device()());  
  uniform_int_distribution<int> dist(0, 4);
  uniform_int_distribution<int> dist2(0, 4);
  int choose = 1;//dist2(mt);

  History<std::string> history;
    
  do {
    cout << "-->>  S T A R T I N G  <<--" << endl;
    mt.seed(random_device()());
    sel0 = dist(mt);
    sel1 = dist(mt);
    sel2 = dist(mt);
    sel3 = dist(mt);
    sel4 = dist(mt);
      
    TERM colorA("color", &regionA, &colorAtoms[sel0]);
    CLAUSE ccolorA(&colorA, ":-");
    TERM colorB("color", &regionB, &colorAtoms[sel1]); 
    CLAUSE ccolorB(&colorB, ":-");
    TERM colorC("color", &regionC, &colorAtoms[sel2]);
    CLAUSE ccolorC(&colorC, ":-");
    TERM colorD("color", &regionD, &colorAtoms[sel3]);
    CLAUSE ccolorD(&colorD, ":-");
    TERM colorE("color", &regionE, &colorAtoms[sel4]);
    CLAUSE ccolorE(&colorE, ":-");

    if (choose == 0) {
      PROGRAM combination(&ccolorA, &ccolorB, &ccolorC, &ccolorD, &ccolorE,
			  &cadj01, &cadj02, &cadj03, &cadj04, &cadj05, &cadj06, &cadj07, &cadj08,
			  &cadj09, &cadj10, &cadj11, &cadj12, &cadj13, &cadj14, &cadj15, &cadj16,
			  &conflict1);
      ATOM conflictTest("conflict");
      success = solve(combination, 0, conflictTest, history);
    }
    else if (choose == 1) {
      PROGRAM combination(&ccolorA, &ccolorB, &ccolorC, &ccolorD, &ccolorE,
			  &cadj01, &cadj02, &cadj03, &cadj04, &cadj05, &cadj06, &cadj07, &cadj08,
			  &cadj09, &cadj10, &cadj11, &cadj12, &cadj13, &cadj14, &cadj15, &cadj16,
			  &conflict2);
      ATOM conflictTest("conflict");
      success = solve(combination, 0, conflictTest, history);
    }
    else if (choose == 2) {
      PROGRAM combination(&ccolorA, &ccolorB, &ccolorC, &ccolorD, &ccolorE,
			  &cadj01, &cadj02, &cadj03, &cadj04, &cadj05, &cadj06, &cadj07, &cadj08,
			  &cadj09, &cadj10, &cadj11, &cadj12, &cadj13, &cadj14, &cadj15, &cadj16,
			  &conflict3);
      ATOM conflictTest("conflict");
      success = solve(combination, 0, conflictTest, history);
    }
    else if (choose == 3) {
      PROGRAM combination(&ccolorA, &ccolorB, &ccolorC, &ccolorD, &ccolorE,
			  &cadj01, &cadj02, &cadj03, &cadj04, &cadj05, &cadj06, &cadj07, &cadj08,
			  &cadj09, &cadj10, &cadj11, &cadj12, &cadj13, &cadj14, &cadj15, &cadj16,
			  &conflict4);
      ATOM conflictTest("conflict");
      success = solve(combination, 0, conflictTest, history);
    }
  } while (success == ResultType::SUCCESS);
  if (success == ResultType::SUCCESS) cout << "Test 11a (map color) Result SUCCESS: There is a conflict. Adjacent colors found" << endl;
  else if (success == ResultType::NOT_FOUND) cout << "Test 11a (map color) not found" << endl;
  else cout << "Test 11a: Result: FAILED_TO_MATCH. No conflict. Current set Ok. No adjacent colors. Arrangement: " << choose << endl;
  cout << "Set used: "<< colorAtoms[sel0].get_functor() << " "<< colorAtoms[sel1].get_functor() << " "<< colorAtoms[sel2].get_functor() << " "<< colorAtoms[sel3].get_functor() << " "<< colorAtoms[sel4].get_functor() << " "<<endl;
  string a_(colorAtoms[sel0].get_functor());
  string b_(colorAtoms[sel1].get_functor());
  string c_(colorAtoms[sel2].get_functor());
  string d_(colorAtoms[sel3].get_functor());
  string e_(colorAtoms[sel4].get_functor());
  CHECK_NE(a_, b_); CHECK_NE(b_, c_);
  CHECK_NE(a_, c_); CHECK_NE(b_, d_);
  CHECK_NE(a_, d_); CHECK_NE(c_, d_);
  CHECK_NE(a_, e_); CHECK_NE(d_, e_);

  
  end = chrono::system_clock::now();

  chrono::duration<double, micro> duration = end - start;

  cout << duration.count()/1000 << "ms " << endl;
}


TEST_CASE("mapcolor2 (not working!)")
{
  using namespace single_header_prolog;
  using namespace std;
  
  /*
+-----+------+
|     | b    |
|  a  +------+
|     |   c  |
+-----+------+

adjacent(regA, regB).
adjacent(regA, regC).
adjacent(regB, regC).
adjacent(regB, regA).
adjacent(regC, regA).
adjacent(regC, regB).

color(regA, blue).
color(regB, green).
color(regC, green).

conflict :- 
    color(Y, C),
    color(X, C),
    adjacent(X, Y).

  */
  
  ATOM regionA("reg_a");
  ATOM regionB("reg_b");
  ATOM regionC("reg_c");

  TERM adj01("adjacent", &regionA, &regionB);       
  CLAUSE cadj01(&adj01, ":-");
  TERM adj02("adjacent", &regionA, &regionC);
  CLAUSE cadj02(&adj02, ":-");  
  TERM adj05("adjacent", &regionB, &regionC);
  CLAUSE cadj05(&adj05, ":-");
  TERM adj09("adjacent", &regionB, &regionA);
  CLAUSE cadj09(&adj09, ":-");
  TERM adj10("adjacent", &regionC, &regionA);
  CLAUSE cadj10(&adj10, ":-");
  TERM adj13("adjacent", &regionC, &regionB);
  CLAUSE cadj13(&adj13, ":-");
  ATOM mapcolorHead("conflict");
  VARIABLE MapX("MapX");
  VARIABLE MapY("MapY", MapX);
  TERM adjacent("adjacent", &MapX, &MapY);
  VARIABLE Color("Color", MapX);
  TERM color1("color", &MapX, &Color);
  TERM color2("color", &MapY, &Color);
  CLAUSE conflict1(&mapcolorHead, ":-", &adjacent, &color1, &color2);  
  CLAUSE conflict2(&mapcolorHead, ":-", &color1, &adjacent, &color2);
  CLAUSE conflict3(&mapcolorHead, ":-", &color2, &color1, &adjacent);
  CLAUSE conflict4(&mapcolorHead, ":-", &color1, &adjacent, &color2);  
  array<ATOM, 3> colorAtoms { {ATOM("\e[1;31mred\e[0m"), ATOM("\e[1;32mgreen\e[0m"), ATOM("\e[1;34mblue\e[0m")} };

  ResultType success = ResultType::UNKNOWN;
    
  int sel0=2, sel1=1, sel2=1; // default vals for debugging

  static mt19937 mt;
  mt.seed(random_device()());  
  uniform_int_distribution<int> dist(0, 2);
  uniform_int_distribution<int> dist2(0, 4);
  int choose = 2;//dist2(mt);

  History<std::string> history;
  
  do {
    cout << "-->>  S T A R T I N G  <<--" << endl;
    mt.seed(random_device()());
    //    sel0 = dist(mt);
    //    sel1 = dist(mt);
    //    sel2 = dist(mt);
      
    TERM colorA("color", &regionA, &colorAtoms[sel0]);
    CLAUSE ccolorA(&colorA, ":-");
    TERM colorB("color", &regionB, &colorAtoms[sel1]); 
    CLAUSE ccolorB(&colorB, ":-");
    TERM colorC("color", &regionC, &colorAtoms[sel2]);
    CLAUSE ccolorC(&colorC, ":-");

    if (choose == 0) {
      PROGRAM combination(&ccolorA, &ccolorB, &ccolorC,
			  &cadj01, &cadj02, &cadj05,
			  &cadj09, &cadj10, &cadj13,
			  &conflict1);
      ATOM conflictTest("conflict");
      success = solve(combination, 0, conflictTest, history);
    }
    else if (choose == 1) {
      PROGRAM combination(&ccolorA, &ccolorB, &ccolorC,
			  &cadj01, &cadj02, &cadj05,
			  &cadj09, &cadj10, &cadj13,			  			  
			  &conflict2);
      ATOM conflictTest("conflict");
      success = solve(combination, 0, conflictTest, history);
    }
    else if (choose == 2) {
      PROGRAM combination(&ccolorA, &ccolorB, &ccolorC,
			  &cadj01, &cadj02, &cadj05,
			  &cadj09, &cadj10, &cadj13,			  			  
			  &conflict3
			  );
      ATOM conflictTest("conflict");
      success = solve(combination, 0, conflictTest, history);
    }
    else if (choose == 3) {
      PROGRAM combination(&ccolorA, &ccolorB, &ccolorC,
			  &cadj01, &cadj02, &cadj05,
			  &cadj09, &cadj10, &cadj13,			  			  
			  &conflict4);
      ATOM conflictTest("conflict");
      success = solve(combination, 0, conflictTest, history);
    }
    //    history.unbind();
  } while (success == ResultType::SUCCESS);
  if (success == ResultType::SUCCESS) cout << "Test 11b (map color2) There should be three different colors, but we have only one or two!" << endl;
  else if (success == ResultType::NOT_FOUND) cout << "Test 11b (map color2) not found" << endl;
  else cout << "Test 11b: Result Ok: All three colors are different " << choose << endl;
  cout << "Set used: "<< colorAtoms[sel0].get_functor() << " "<< colorAtoms[sel1].get_functor() << " "<< colorAtoms[sel2].get_functor() << " " <<endl;
  string a_(colorAtoms[sel0].get_functor());
  string b_(colorAtoms[sel1].get_functor());
  string c_(colorAtoms[sel2].get_functor());
  CHECK_NE(a_, b_); CHECK_NE(b_, c_);
  CHECK_NE(a_, c_); 
}


TEST_CASE("scheduling")
{
  using namespace single_header_prolog;
  using namespace std;

  ATOM monday("\e[1;31mmonday\e[0m");
  ATOM tuesday("\e[1;34mtuesday\e[0m");
  ATOM wednesday("\e[1;33mwednesday\e[0m");
  TERM before1("before", &monday, &tuesday);
  CLAUSE cbefore1(&before1, ":-");
  TERM before2("before", &tuesday, &wednesday);
  CLAUSE cbefore2(&before2, ":-");
  ATOM take_walk("take_walk");
  TERM i_will_b("i_will", &take_walk, &tuesday); // We know this fact (I walk on tuesday), the rest of the days
  CLAUSE ci_will_b(&i_will_b, ":-");             // must be searched for the rest of the days! (Haha!)
  ATOM shop("shop");
  ATOM barber("barber");
  ATOM market("market");
  VARIABLE DayX("DayX");
  TERM event1("i_will", &market, &DayX); // The same day I go to the market I go shopping
  TERM event2("i_will", &shop, &DayX);
  VARIABLE DayY("DayY");
  TERM event3("i_will", &take_walk, &DayY); // The same day I go for a walk I go to the barber
  TERM event4("i_will", &barber, &DayY);
  TERM beforeCheck("before", &DayX, &DayY); // I go to the market the day before I take a walk...
  ATOM scheduleHead("check");
  CLAUSE cschedule(&scheduleHead, ":-", &event1, &event2, &event3, &event4, &beforeCheck);
  array<ATOM*, 3> weekdays { {&monday, &tuesday, &wednesday} };

  ResultType success;

  int sel0, sel1, sel2;

  History<std::string> history;
  
  do {
    static mt19937 mt;
    mt.seed(random_device()());
    uniform_int_distribution<int> dist(0, 2);
    sel0 = dist(mt);
    sel1 = dist(mt);
    sel2 = dist(mt);

    TERM i_will_a("i_will", &shop, weekdays[sel0]);
    CLAUSE ci_will_a(&i_will_a, ":-");
    // We already know b ^^
    TERM i_will_c("i_will", &barber, weekdays[sel1]);
    CLAUSE ci_will_c(&i_will_c, ":-");
    TERM i_will_d("i_will", &market, weekdays[sel2]);
    CLAUSE ci_will_d(&i_will_d, ":-");

    PROGRAM schedule(&ci_will_a, &ci_will_b, &ci_will_c, &ci_will_d, &cbefore1, &cbefore2, &cschedule);
    ATOM scheduleTest("check");
    success = solve(schedule, 0, scheduleTest, history);

    history.reset();
  } while (success != ResultType::SUCCESS);
  if (success == ResultType::SUCCESS) cout << "Test 12 (schedule) Result SUCCESS." << endl;
  else if (success == ResultType::NOT_FOUND) cout << "Test 12 (schedule) not found" << endl;
  else cout << "Test 12: Result: FAILED_TO_MATCH." << endl;
  cout << "Set used: shop: " << weekdays[sel0]->get_functor() << " take a walk: " << tuesday.get_functor() << " visit barber: " << weekdays[sel1]->get_functor() << " market: " << weekdays[sel2]->get_functor() << endl;
  REQUIRE(weekdays[sel0]->get_functor() == monday.get_functor());
  REQUIRE(weekdays[sel1]->get_functor() == tuesday.get_functor());
  REQUIRE(weekdays[sel2]->get_functor() == weekdays[sel0]->get_functor());
}


TEST_CASE("nested")
{
  using namespace single_header_prolog;
  using namespace std;

  ATOM endpoint("endpoint");
  
  TERM   pickupT("pickup", &endpoint);
  CLAUSE pickupC(&pickupT, ":-");

  ATOM   shcA("endpoint");
  TERM   shcT("shc", &shcA);
  CLAUSE shcC(&shcT, ":-");

  ATOM   scaA("endpoint");
  TERM   scaT("sca", &scaA);
  CLAUSE scaC(&scaT, ":-");

  ATOM   sstA("endpoint");
  TERM   sstT("sst", &sstA);
  CLAUSE sstC(&sstT, ":-");
  
  VARIABLE A("A");
  TERM   cscaC1("sca", &A);
  TERM   csstC1("sst", &A);
  TERM   spoC1Head("spo", &A);
  CLAUSE spoC1(&spoC1Head, ":-", &cscaC1, &csstC1);

  ATOM wrongpoint("wrongpoint");
  TERM   spoT2("spo", &wrongpoint);
  CLAUSE spoC2(&spoT2, ":-");
  
  VARIABLE X("X");
  TERM cpickup("pickup", &X);
  TERM cshc("shc", &X);
  TERM cspo("spo", &X);
  ATOM completedHead("completed");
  CLAUSE completed(&completedHead, ":-", &cpickup, &cshc, &cspo);

  PROGRAM nested(&pickupC, &shcC, &scaC, &sstC, &spoC1, &spoC2, &completed);

  History<std::string> history;
  
  auto success = solve(nested, 0, completedHead, history);
  REQUIRE(success == ResultType::SUCCESS);
  REQUIRE_EQ(A.get_binding()->get_functor(), string("endpoint"));
}




TEST_CASE("deep")
{
  using namespace single_header_prolog;
  using namespace std;

  /* Testcase 'deep':

  small(rA).
  big(rB).
  big(rC).

  isLeftTo(rA, rB).
  isLeftTo(rB, rC).

  nei(X, Y) :- isLeftTo(X, Y).

  lll :- 
      big(Xlll),
      big(Ylll),
      small(Zlll),
      nei(Xlll, Ylll).
    
  valid :- lll.  

  */
  
  Atom<string> irAa(string("rA"));
  Term<string> irAt(string("small"), &irAa);
  Clause<string> irAc(&irAt, string(":-"));

  Atom<string> irBa(string("rB"));
  Term<string> irBt(string("big"), &irBa);
  Clause<string> irBc(&irBt, string(":-"));

  Atom<string> irCa(string("rC"));
  Term<string> irCt(string("big"), &irCa);
  Clause<string> irCc(&irCt, string(":-"));
 
  Atom<string> rA(string("rA"));
  Atom<string> rB(string("rB"));
  Atom<string> rC(string("rC"));
  Term<string> isLeftTo1(string("leftto"), &rA, &rB);
  Clause<string> ilt1(&isLeftTo1, string(":-"));
  
  Term<string> isLeftTo2(string("leftto"), &rB, &rC);
  Clause<string> ilt2(&isLeftTo2, string(":-"));

  Variable<string> X(string("X"));
  Variable<string> Y(string("Y"), X);
  Term<string> isnextto(string("leftto"), &X, &Y);
  Term<string> neihead(string("nei"), &X, &Y);
  Clause<string> neivors(&neihead, string(":-"), &isnextto);
  
  Variable<string> X1(string("Xlll"));
  Variable<string> Y1(string("Ylll"), X1);
  Variable<string> Z1(string("Zlll"), X1);
  Term<string> t1(string("big"), &X1);
  Term<string> t2(string("big"), &Y1);
  Term<string> t3(string("small"), &Z1);
  Term<string> t4(string("nei"), &X1, &Y1);
  Atom<string> hh(string("lll"));
  Clause<string> tt(&hh, string(":-"), &t1, &t2, &t3, &t4);
  
  Atom<string> h2(string("lll"));			    
  Atom<string> head(string("valid"));
  Clause<string> valid(&head, string(":-"), &h2);

  Program<string> program { &irAc, &irBc, &irCc, &ilt1, &ilt2, &neivors, &tt, &valid };

  History<string> history;
  auto res = ResultType::UNKNOWN;
  Atom<string> start(string("valid"));
  res = solve(program, 0, start, history);
  cout << "\e[1;35mres: " << (int)res << " " << history.force_skip << " " << "\e[0m"<<endl;

  CHECK(res == ResultType::SUCCESS);
  
  history.reset();
  X.reset();
  Y.reset();
  X1.reset();
  Y1.reset();
  Z1.reset();
  
  history.force_skip += 1;
  res = solve(program, 0, start, history);
  cout << "\e[1;36mres: " << (int)res << " " << history.force_skip << " " << "\e[0m"<<endl;

  CHECK(res == ResultType::FAILED_TO_MATCH);  
}




TEST_CASE("negation")
{
  using namespace single_header_prolog;
  using namespace std;

  /* enjoys(vincent,X)  :-  big_kahuna_burger(X),!,fail. */
  
  ATOM   vincent("vincent");

  VARIABLE  varX("X");
  TERM   enjoysh("enjoys", &vincent, &varX);
  TERM     body1("big_kahuna_burger", &varX);
  Cut<string> cutA("!");
  Fail<string> failT("fail");
  CLAUSE enjoys1(&enjoysh, ":-", &body1, &cutA, &failT);

  VARIABLE  varY("Y");
  TERM   enjoysH("enjoys", &vincent, &varY);
  TERM     body2("burger", &varY);
  CLAUSE enjoys2(&enjoysH, ":-", &body2);        

  VARIABLE x1("X1");
  TERM bh1("burger", &x1);
  TERM bigmac("big_mac", &x1);
  CLAUSE burger1(&bh1, ":-", &bigmac);

  VARIABLE x2("X2");
  TERM bh2("burger", &x2);
  TERM bigkahuna("big_kahuna_burger", &x2);
  CLAUSE burger2(&bh2, ":-", &bigkahuna);

  VARIABLE x3("X3");
  TERM bh3("burger", &x3);
  TERM whopper("whopper", &x3);
  CLAUSE burger3(&bh3, ":-", &whopper);

  ATOM a("a");
  ATOM b("b");
  ATOM c("c");

  TERM   big_macT("big_mac", &a);
  CLAUSE big_macC(&big_macT, ":-");

  TERM   big_kahuna_burgerT("big_kahuna_burger", &b);
  CLAUSE big_kahuna_burgerC(&big_kahuna_burgerT, ":-");

  TERM   whopperT("whopper", &c);
  CLAUSE whopperC(&whopperT, ":-");


  ATOM valid("valid");
  TERM queryEnjoysA("enjoys", &vincent, &a); // Yes/true
  CLAUSE queryEnjoysAC(&valid, ":-", &queryEnjoysA);

  ATOM valid2("valid2");
  TERM queryEnjoysB("enjoys", &vincent, &b); // No/false
  CLAUSE queryEnjoysBC(&valid2, ":-", &queryEnjoysB);

  ATOM valid3("valid3");
  TERM queryEnjoysC("enjoys", &vincent, &c); // Yes/true
  CLAUSE queryEnjoysCC(&valid3, ":-", &queryEnjoysC);

  
  Cut<string>     cut("!");  CLAUSE  cutC(&cut, ":-");
  Fail<string> failT2("fail"); CLAUSE failC(&failT2, ":-");
  PROGRAM program { &enjoys1, &enjoys2, &burger1, &burger2, &burger3, &big_macC, &big_kahuna_burgerC, &whopperC, &cutC, &failC,
                    &queryEnjoysAC, &queryEnjoysBC, &queryEnjoysCC };
  
  History<string> history1;
  auto res = solve(program, 0, valid, history1);
  cout << (int)res << endl;
  CHECK((int)res == 0);
  varX.reset(); varY.reset(); x1.reset(); x2.reset(); x3.reset();
  
  
  History<string> history2;
  res = solve(program, 0, valid2, history2);
  cout << (int)res << endl;
  CHECK((int)res == 1);
  varX.reset(); varY.reset(); x1.reset(); x2.reset(); x3.reset();

  
  History<string> history3;
  res = solve(program, 0, valid3, history3);
  cout << (int)res << endl;
  CHECK((int)res == 0);
}



TEST_CASE("select")
{
  using namespace single_header_prolog;
  using namespace std;

/*
green(cell0).
green(cell1).
green(cell2).

big(cell0).
big(cell1).
small(cell2).

neg(Goal)  :-  Goal,!,fail. 
neg(_).

ok :- green(X), neg(big(X)). 
                                   
// Every 'green' should be 'big'
valid :- neg(ok). // There are two ways in which this can fail:
                  // 1. We find a 'green' which is not 'big' or             
                  // 2. we reach the end.
*/

  ATOM cell0("cell0");
  ATOM cell1("cell1");
  ATOM cell2("cell2");

  TERM    green0("green", &cell0);
  CLAUSE green0C(&green0, ":-");
  TERM    green1("green", &cell1);
  CLAUSE green1C(&green1, ":-");
  TERM    green2("green", &cell2);
  CLAUSE green2C(&green2, ":-");

  TERM    big0("big", &cell0);
  CLAUSE big0C(&big0, ":-");
  TERM    big1("big", &cell1);
  CLAUSE big1C(&big1, ":-");
  TERM    sml2("small", &cell2);
  CLAUSE sml2C(&sml2, ":-");

  VARIABLE      goal("Goal");
  TERM         neg1H("\\+", &goal);
  Cut<string>   cutA("!");
  Fail<string> failT("fail");
  CLAUSE       neg1C(&neg1H, ":-", &goal, &cutA, &failT);

  //  Placeholder<string> ph("_");
  VARIABLE ph("_");
  TERM           neg2H("\\+", &ph);
  CLAUSE         neg2C(&neg2H, ":-");

  VARIABLE X   ("X");
  ATOM     okH ("ok");
  TERM     okB0("green", &X);
  TERM     bigT("big", &X);  
  TERM     negT("\\+", &bigT);
  CLAUSE   okC (&okH, ":-", &okB0, &negT);

  ATOM  valid("valid");
  ATOM     ok("ok");
  //  TERM  negOK("\\+", &ok);
  CLAUSE okC2(&valid, ":-", &ok);

  ATOM  valid2("valid2");
  ATOM     ok2("ok");
  TERM  negOK2("\\+", &ok2);
  CLAUSE  okC3(&valid2, ":-", &negOK2);

  
  Cut<string>   cut("!");    CLAUSE  cutC(&cut, ":-");
  Fail<string> fail("fail"); CLAUSE failC(&fail, ":-");
  PROGRAM program { &green0C, &green1C, &green2C, &big0C, &big1C, &sml2C, &okC, &neg1C, &neg2C, &okC2, &okC3, &cutC, &failC };


  History<string> history;
  auto res = solve(program, 0, valid, history); // Calling 'valid' should result in 'SUCCESS'
  cout << (int)res << endl;
  CHECK(res == ResultType::SUCCESS);

  History<string> history2;
  res = solve(program, 0, valid2, history2); // Calling 'valid2' should result in 'FAILED_TO_MATCH'
  cout << (int)res << endl;
  CHECK(res == ResultType::FAILED_TO_MATCH);
}



TEST_CASE("compatible")
{
  using namespace single_header_prolog;
  using namespace std;

  ATOM sam("sam");
  ATOM dal("dal");
  ATOM curry("curry");
  ATOM rajiv("rajiv");
  ATOM burgers("burgers");
  ATOM josie("josie");
  ATOM samosas("samosas");
  TERM eats00("eats", &sam, &dal);
  CLAUSE eats00C(&eats00, ":-");
  TERM eats01("eats", &sam, &curry);
  CLAUSE eats01C(&eats01, ":-");
  TERM eats02("eats", &rajiv, &burgers);
  CLAUSE eats02C(&eats02, ":-");
  TERM eats03("eats", &josie, &samosas);
  CLAUSE eats03C(&eats03, ":-");
  TERM eats04("eats", &josie, &curry);
  CLAUSE eats04C(&eats04, ":-");
  TERM eats05("eats", &rajiv, &dal);
  CLAUSE eats05C(&eats05, ":-");
  
  VARIABLE Person1("Person1");
  VARIABLE Person2("Person2", Person1);  
  TERM compatibleHd("compatible", &Person1, &Person2);
  VARIABLE Food("Food", Person1);
  TERM eats_body1("eats", &Person1, &Food);
  TERM eats_body2("eats", &Person2, &Food);
  CLAUSE compatible(&compatibleHd, ":-", &eats_body1, &eats_body2);  

  PROGRAM knowledge_base(&eats00C, &eats01C, &eats02C, &eats03C, &eats04C, &eats05C, &compatible);

    VARIABLE X("X");
    ATOM rajiv_("rajiv");
    TERM query("compatible", &rajiv_, &X);

    History<std::string> history;

    ResultType success = ResultType::UNKNOWN;
    
    success = solve(knowledge_base, 0, query, history);
    CHECK(X.get_binding()->get_functor() == "sam");
    cout << "Test 'compatible': rajiv is compatible w/ " << X.get_binding()->get_functor() << endl;

    history.unbind();

    //    history.force_skip = 1;    
    success = solve(knowledge_base, 0, query, history);
    CHECK(X.get_binding()->get_functor() == "curry");
}




struct Carrier {
  explicit Carrier(const std::string& str, const std::initializer_list<int>& solutions)
    : payload(str),
      permittedSolutions(solutions)
  {}

  bool add_solution(int solId)
  {
    if (permittedSolutions.find(solId) != permittedSolutions.end()) {
      proposedSolutions.insert(solId);
      return true;
    }
    else {
      return false;
    }
  }

  bool execute(const std::unordered_map<int, std::function<bool(Carrier&)>>& solutions)
  {
    for (const auto& it : proposedSolutions) {
      const auto fun = solutions.find(it);
      if (fun == solutions.end()) { 
	proposedSolutions.erase(it);
	return true;
      }

      if (fun->second(*this)) {
	proposedSolutions.erase(it);
	continue;
      }
      else {
	return false;
      }
    }
    return true;
  }
  
  mutable std::string payload;
  mutable bool done_printing = false;
  
  const std::unordered_set<int> permittedSolutions;
  std::unordered_set<int> proposedSolutions;
};

  
struct Proposal {
  Proposal()
    : id(-1),
      verifier([](Carrier*){ return false; })
  {}
  
  //    Proposal(const Proposal&) = default;
  //    Proposal& operator=(const Proposal&) = default;
  
  explicit Proposal(int nme)
    : id(nme),
      verifier([](Carrier*){ return false; })
  {}
  
  explicit Proposal(int nme, std::function<bool(Carrier*)>&& very)
    : id(nme),
      verifier(move(very))
  {}

  explicit Proposal(int nme, Carrier* cr_ptr)
    : id(nme),
      verifier([](Carrier*){ return false; }),
      carrier_ptr(cr_ptr)
  {}
  
  bool operator==(const Proposal& other) const
  {
    bool suc = (id == other.id);
    if (suc) {
      if (carrier_ptr != nullptr) {
	suc = other.verifier(carrier_ptr);
      } 
    }
    return suc;
  }
  
  bool operator!=(const Proposal& other) const
  {
    return id != other.id;
  }
  
  int id; // todo const?
  std::function<bool(Carrier*)> verifier;
  Carrier* carrier_ptr = nullptr;
};


std::ostream& operator<<(std::ostream& os, const Proposal& x) { os << std::hex << x.id; return os; }




TEST_CASE("walker")
{
  using namespace single_header_prolog;
  using namespace std;
  
  
  constexpr int fnGoal                        = 0x01;
  constexpr int fnCompleted                   = 0x20;
  constexpr int fnPickupPayload               = 0x05;
  constexpr int fnShouldHaveTwoAsInTheMiddle  = 0x2f;
  constexpr int fnShouldEndWithX              = 0x03;
  constexpr int fnSub1a                       = 0x31;
  constexpr int fnSub2a                       = 0x32;
  constexpr int fnPrinter                     = 0x33;

  constexpr int makeTwoAsInTheMiddle = 0xf0;
  constexpr int makeBigX             = 0xf1;
  constexpr int makeSmallX           = 0xf2;
  constexpr int doABackflip          = 0xf3;
  constexpr int doPrint              = 0xe0;

  int iter = 0;
  
  Carrier carrier { string("......"), { makeTwoAsInTheMiddle, makeSmallX,
	                                doABackflip, doABackflip+1, doABackflip+2, doABackflip+3, doABackflip+4,
	                                doABackflip+5, doABackflip+6, doABackflip+7, doABackflip+8, doABackflip+8,
	                                doABackflip+9, doABackflip+10, doABackflip+11, doPrint } }; // 'makeBigX' is not allowed

  Atom<Proposal>   goal0(Proposal{ fnGoal, &carrier });
  Term<Proposal>      t0(Proposal{ fnPickupPayload   }, &goal0);
  Clause<Proposal>    c0(&t0, Proposal{ 0                 });

  Variable<Proposal>    goalVar(Proposal{ 0               }          );
  Term<Proposal>     do_stuff_0(Proposal{ fnPickupPayload }, &goalVar);

  Variable<Proposal> temporaryA(Proposal{ 0 /*, &carrier*/ });

  Atom<Proposal>   goal1a(Proposal{ fnGoal,                      
			            [](Carrier* carrier)
				    {
				      cout << "Checking two A's in the middle..."<<endl;
				      bool res = (carrier->payload[2] == 'A' && carrier->payload[3] == 'A');
				      if (!res) {
					res = carrier->add_solution(makeTwoAsInTheMiddle);
				      }
				      return res;
				    }});
  Term<Proposal>   sub1aT(Proposal{ fnSub1a }, &goal1a);
  Clause<Proposal> sub1aC(&sub1aT, Proposal{ 0 });
  Term<Proposal>   sub1a (Proposal{ fnSub1a }, &temporaryA);
  Atom<Proposal>   goal1b(Proposal{ fnGoal,
	                            [&iter](Carrier* carrier)
				    {
				      bool res = iter >= 11;
				      if (!res) {
					res = carrier->add_solution(doABackflip + iter);
					iter++;
				      }
				      return res;
				    }});
  Term<Proposal>   sub2aT(Proposal{ fnSub2a }, &goal1b);
  Clause<Proposal> sub2aC(&sub2aT, Proposal{ 0 }); 
  Term<Proposal>   sub2a (Proposal{ fnSub2a }, &goalVar); //&temporaryA);

  Term<Proposal>  c1Head(Proposal{ fnShouldHaveTwoAsInTheMiddle }, &temporaryA);
  Clause<Proposal>    c1(&c1Head, Proposal{ 0 }, &sub1a, &sub2a, &sub2a, &sub2a, &sub2a, &sub2a, &sub2a, &sub2a, &sub2a, &sub2a, &sub2a, &sub2a);

  Atom<Proposal> c4print(Proposal{ fnGoal, [](Carrier*){ cout << "Printing from c4 " << endl; return true; } });
  Term<Proposal>  c4Head(Proposal{ fnShouldHaveTwoAsInTheMiddle }, &c4print);
  Clause<Proposal>    c4(&c4Head, Proposal{ 0 });
  Term<Proposal>        do_stuff_1(Proposal{ fnShouldHaveTwoAsInTheMiddle }, &goalVar);
  
  Atom<Proposal>   goal2(Proposal{ fnGoal,
	                           [](Carrier* carrier)
				   {
				     cout << "Checking big X..."<<endl;
				     bool res = (carrier->payload[5] == 'X');
				     if (!res) {
				       res = carrier->add_solution(makeBigX);
				     }
				     return res;
				   }});
  Term<Proposal>      t2(Proposal{ fnShouldEndWithX             }, &goal2);
  Clause<Proposal>    c2(&t2, Proposal{ 0 });

  Atom<Proposal>   goal3(Proposal{ fnGoal,
	                           [](Carrier* carrier)
				   {
				     cout << "Checking small x..."<<endl;
				     bool res = (carrier->payload[5] == 'x');
				     if (!res) {
				       res = carrier->add_solution(makeSmallX);
				     }
				     return res;
				   }});
  Term<Proposal>      t3(Proposal{ fnShouldEndWithX             }, &goal3);
  Clause<Proposal>    c3(&t3, Proposal{ 0 });

  Atom<Proposal>   printA(Proposal{ fnGoal,
	                            [](Carrier* carrier)
				    {
				      bool res = carrier->done_printing;
				      if (!res) {
					res = carrier->add_solution(doPrint);
				      }
				      return res;
				    }});
  Term<Proposal>   printT(Proposal{ fnPrinter }, &printA);
  Clause<Proposal> printC(&printT, Proposal{ 0 });
  Term<Proposal>   print (Proposal{ fnPrinter }, &goalVar);
    
  
  Atom<Proposal>     completedHead(Proposal{ fnCompleted                  }          );  
  Term<Proposal>        do_stuff_2(Proposal{ fnShouldEndWithX             }, &goalVar);  
  Clause<Proposal>       completed(&completedHead, Proposal{ 0 },
			    	   &do_stuff_0, &do_stuff_1, &do_stuff_2, &print);
  
  Program<Proposal> modifyStrTestProgram(&sub1aC, &sub2aC, &c0, &c1, &c4, &c2, &c3, &printC, &completed);

  string text;
  
  const unordered_map<int, function<bool(Carrier&)>> solutions {
    { makeTwoAsInTheMiddle, [](Carrier& carrier){ carrier.payload[2] = 'A'; carrier.payload[3] = 'A'; return true; } },
    { makeBigX            , [](Carrier& carrier){ carrier.payload[5] = 'X';                           return true; } },
    { makeSmallX          , [](Carrier& carrier){ carrier.payload[5] = 'x';                           return true; } },
    { doABackflip         , [](Carrier& carrier){ cout << "*flip 1*";                                 return true; } },
    { doABackflip+1       , [](Carrier& carrier){ cout << "*flip 2*";                                 return true; } },
    { doABackflip+2       , [](Carrier& carrier){ cout << "*flip 3*";                                 return true; } },
    { doABackflip+3       , [](Carrier& carrier){ cout << "*flip 4*";                                 return true; } },
    { doABackflip+4       , [](Carrier& carrier){ cout << "*flip 5*";                                 return true; } },
    { doABackflip+5       , [](Carrier& carrier){ cout << "*flip 6*";                                 return true; } },
    { doABackflip+6       , [](Carrier& carrier){ cout << "*flip 7*";                                 return true; } },
    { doABackflip+7       , [](Carrier& carrier){ cout << "*flip 8*";                                 return true; } },
    { doABackflip+8       , [](Carrier& carrier){ cout << "*flip 9*";                                 return true; } },
    { doABackflip+9       , [](Carrier& carrier){ cout << "*flip10*";                                 return true; } },
    { doABackflip+10      , [](Carrier& carrier){ cout << "*flip11*";                                 return true; } },
    { doPrint             , [&text](Carrier& carrier){ carrier.done_printing = true; text = "text printed!"; return true; } }
  };
				       				       

  auto success = ResultType::UNKNOWN;

  History<Proposal> history;

  do {
    success = solve(modifyStrTestProgram, 0, completedHead, history);
    cout << "\e[1;32mOutstanding solutions: " << carrier.proposedSolutions.size() << "\e[0m" << endl;
    if (!carrier.execute(solutions)) {
      CHECK(1 == 2);
      break;
    }

    history.unbind(); // Can be removed

  } while (carrier.proposedSolutions.size() > 0);

  cout << "Success? " << (int)success << endl;

  CHECK_EQ(text, string("text printed!"));
  CHECK(success == ResultType::SUCCESS);
  CHECK(carrier.payload == "..AA.x");
}


