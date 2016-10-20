Logical Concurrency Control From Sequential Proofs 

- This project focuses on the ﬁnding the code portion prone to race conditions using the sequential proofs,

- that ensures that given assertions in the program holds in any sequential execution. After ﬁnding those code

- portions, we have used the proper locking scheme so as to ensure the logical concurrency of the program.

- Given a sequential program annotated with assertions and proofs that ensures,assertion holds in any

- sequential execution of the program,we have used those assertions to synthesize proper locks in the pro-

- gram.Introduction of these locks,ensure that assertions that holds in sequential execution also holds in the

- presence of the concurrent execution,i.e this locking scheme ensure serializabilty.

- For a concurrent library,to ensure consistency between diﬀerent executions one way is to lock all the the

- statement of the program under single lock and other way could be to make each of statement atomic.The

- former one ensure isolation requirements but poor concurrency and with latter, library don’t considers some

- of the safe interleavings between the threads.The paper “Logical Concurrency Control from sequential proofs”

- presents the idea that,what could be the safe interleaving of threads and how to ﬁnd those safe interleaving

- using sequential proofs.

- 2 Paper Summary

- 2.1 Paper Idea

- The paper presents the systematic process of ﬁnding unsafe interleaving of the threads starting form

- sequential proofs so as to meet the “sequential speciﬁcation” of the library. These proofs ensures that given

- program satisﬁes certain assertions in the absence of concurrent client. And the assertions based on these

- proofs model can be used to synthesize the concurrent library. That further ensure these assertions are

- satisﬁed in the presence of the concurrent clients i.e enforcing the consistency constraint on the library.

- In sequential setting the program can be viewed as Control Flow Automata G = (V, E) where V set of

- program points and E set of edges where each edge represent a particular statement in the program.The

- proof µ for the sequential setting , can be viewed as invariant, i.e v  V, satisﬁes µ(v); such that for each

- edge u→v corresponding to the statement, say s,ensures that,given the precondition µ(u) holds,then after the

- execution of s µ(v) also holds. And if s is an assertion ϕ, then µ(u)⇒ ϕ should be valid. In a very broad way

- these constraints ensures that proof is valid for any sequence of execution of the sequential program.“The

- invariant µ(u)at a vertex u indicates the property to hold at u to ensure that a sequential execution satisﬁes

- all assertions of library.”

- Consider two concurrent threads say t1 and t2,the interleaving from thread,say t1(at u) → t2(at ˆu) is

- considered safe if execution of statement ˆs in t2 from ˆu → ˆv till the invariant µ(u) holds in t1. But if the

- execution of ˆs aﬀects µ(u),interleaving needs to be restricted at u,as further execution of the s will fail to

- satisfy the assertion in t1.

- In more atomic sense the invariant µ can be thought of conjunction of boolean predicated say ρi and the

- check is made on predicates rather invariants. If the execution of the statement ˆs of one thread invalidates

- the predicate ρj of the other thread of invariant µ(u),then locking u with the lock (cid:96)ρj will allow only those

- threads to execute ˆs that acquire lock (cid:96)ρj . With this locking scheme we are ensuring isolation constraints

- over those statements which are prone to race condition basically unsafe interleaving.

- 2.2 Optimization

- It may be possible that locking scheme here used can lead to redundant locks.e.g lock (cid:96)1 is always held

- when lock (cid:96)2 is acquired by any thread then lock (cid:96)1 is redundant.Or it may be possible that particular

- statement don’t invalidates any predicate in the library,then there is no need for having the lock for that

- predicate.Further the idea of reader-writer lock can also be introduced so as to increase the concurrency.

- 2.3 Example

- Consider the following C code snippet.

- int x , y ;

- int f o o ( int i )

- {

- x=x+i ;

- int j ;

- j =1;

- x=x+1;

- j=x+y ;

- return j ;

// x=∗

// x=x+i

// x=x+1

// j=x+y

- }

- If we consider the sequential execution of the above code,then we get the proofs in form of hoare triple e.g

- for the statement ’4’ {x = xin}x = xin + i{x = xin + i} forms a valid hoare triple. In concurrent library,these

- triples are used as the basis to ﬁnd the proper locking scheme.Say t1 and t2 are two diﬀerent threads at u is

- 4 and ˆu is 7 respectively.

- Consider the interleaving from t1 → t2,then.Execution of t2 invalidates the precondition {x = xin} at u.

- From t1 perspective {x = xin + 1}x = xin + i{x = xin + i} don’t forms a valid hoare triple,since it is not

- satisﬁable for any value of xin.

- From the idea of the algorithm this is the implication that such of the interleaving must be controlled.If

- the statement u is held with a lock (cid:96)x=xin then t2 will not be allowed to execute ˆu until it acquire (cid:96)x=xin .Since

- the thread t1 holds the write lock at u other thread t2 will not be allowed go through the statement ˆu. This

- ensure the atomicity of those statements which are dependent.

- The Similar argument can be given for the statement 8 of t1 with the statement 4 of thread t2. Since the

- predicate {j = x + y} is falsiﬁed.So the statement 8 and 4 are locked with (cid:96)j=x+y. So the Concurrent code

- with the locks looks like :

- int x , y ;

- int f o o ( int i )

- {

r e l e a s e ( l 2 ) ;

- a c q u i r e ( l 1 ) ; a c q u i r e ( l 2 ) ;

- x=x+i ;

- r e l e a s e ( l 1 ) ;

- int j ;

- j =1;

- a c q u i r e ( l 1 ) ;

- x=x+1;

- r e l e a s e ( l 1 ) ; a c q u i r e ( l 2 ) ;

- j=x+y ;

- r e l e a s e ( l 2 ) ;

- return j ;

- }

- The above output code can be further optimized,like the lock (cid:96)2 is always acquired either after the lock (cid:96)1

- is acquired or it has been released.Giving the intuition that both the locks can be merged without aﬀecting

- the correctness.

- int f o o ( int i )

- {

- a c q u i r e ( l 1 2 ) ;

- x=x+i ;

- r e l e a s e ( l 1 2 ) ;

- int j ;

- j =1;

- a c q u i r e ( l 1 2 ) ;

- x=x+1;

- j=x+y ;

- r e l e a s e ( l 1 2 ) ;

- return j ;

- }

- 3 Implementation

- To make the prototype implementation of the paper’s algorithms we have used CPAChecker,Z3 SMT solver

- tools.The Process starts by getting the input i.e sequential proofs from CPAChecker. It is model checking

- tool that provide us with the set of prediactes for each program point,that are used as the basis as sequential

- proofs what we called as µ throughout. After ﬁnding those proofs. Algorithm presented in the paper then

- picks up a prediactes one after the other and checks whether or not the predicate is falsiﬁed,if yes then by

- which statement make that falsiﬁcation. Now the statement is locked with that prediacte. Then our imple-

- mentation uses these proofs and the sequential program to synthesize the concurrency control for the library.

- Figure1 is the High Level of the entire process model.

- Sequential library is given as input to the cpachecker.The cpachecker is capable enough to obtain sequential

- proofs ,predicates on the basis of some of the assertions.These assertions are fed manually to the program.

- Since these proofs have dependency over these assertions,so we got to choose those assertions in the way

- that required property of the proof holds.

- These proofs with the CFA are fed the algorithm presented in the paper. This prototype algorithm does

- the select a predicate from the proof’s set and chcek it with all the statements in the program.Simply checking

- the validity of predicate after running the statement. This validity is checked using Hoare logic tool.If the

- statement even after execution satisﬁes the sequential speciﬁcation then that statement is not locked. But

- if the speciﬁcation are falsiﬁed then the statement is locked with the name of that predicate.

- Z3 SMT solver is used to check whether {µ(u)∧ ϕ} s  E {ϕ} is valid hoare triple or not. Z3 provides C++

- interface that is harnessed to check for the validity of the hoare triple.Intuitively we want {µ(u)∧ ϕ} and the

- statement ’s’ to get some post condition using the hoare logic for some,say assignment statement. And also

![Im1](images/Im1)

- we know that {precondition}statement{postcondition} forms a valid hoare triple,then it must imply our

- assertion/predicate statement ϕ. So if the post condition we obtained using the hoare logic of assignment

- statement implies our assertion then we are done.

- The entire set of haore tripple that we need to check for,are obatined using using the protoype algorithm

- and saved into a particular ﬁle. And this ﬁle is given to Z3Smt solver to check for the valididty of the

- implication of prediacte.

- The ﬁnal set of locks with names of prediactes gives the required concurrent libarary.

- 4 Conclusion

- The idea presented in the paper don’t focuses on ﬁnding the lock based synchronization to ensure atomicity.

- Rather presented idea,focuses to avoid those interleaving that can make sequential speciﬁcation invalid.So

- this locking scheme provides the lock in such a way that when the concurrent library is executed in the

- presence of these locks then all the sequential assertions holds at the end.

- The idea is fairly simple and eﬀective.And have been tested for variety of programs.The output concurrent

- library is eﬃcient and correct for those tested programs.

