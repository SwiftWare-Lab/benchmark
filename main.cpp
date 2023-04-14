//
// Created by Kazem on 2023-04-11.
//

#include <iostream>

#include <benchmark/benchmark.h>

struct Inputs{
};

struct Outputs{
};

struct Stats{
};



class TestingBench{
protected:
 Inputs *In;
 Outputs *Out;
 Stats *St;


 virtual void setup(){
  std::cout<<"TestingBench::setup()"<<std::endl;
 }

 virtual void inspect(){
  std::cout<<"TestingBench::analysis()"<<std::endl;
 }

 virtual double test(){
  std::cout<<"TestingBench::test()"<<std::endl;
 }

 virtual void teardown(){
  std::cout<<"TestingBench::teardown()"<<std::endl;
 }

 public:

 TestingBench(Inputs *In1, Outputs *Out1, Stats *St1):
   In(In1), Out(Out1), St(St1){}

 void run(){
  setup();
  inspect();
  test();
  teardown();
 }
};


class Test1: public TestingBench{

protected:
 void setup() override{
  std::cout<<"Test1::setup()"<<std::endl;
 }

 void inspect() override{
  std::cout<<"Test1::analysis()"<<std::endl;
 }

 double test() override{
  double tmp = 0;
  for (int i = 0; i < 100; ++i) {
   tmp = 0;
   for (int j = 0; j < 100; ++j) {
    tmp *= j;
   }
   tmp += i;
  }
  std::cout<<"Test1::test()"<<tmp<<std::endl;
 }

 void teardown() override {
  std::cout<<"Test1::teardown()"<<std::endl;
 }

public:
 Test1(Inputs *In1, Outputs *Out1, Stats *St1):
   TestingBench(In1, Out1, St1){}

};


static void bmSomeFunction(benchmark::State& State) {
 // Perform setup here
 for (auto _ : State) {
  // This code gets timed
  double tmp = 0;
  for (int i = 0; i < 100; ++i) {
   tmp = 0;
   for (int j = 0; j < 100; ++j) {
    tmp *= j;
   }
   tmp += i;
  }
 }
}
// Register the function as a benchmark
BENCHMARK(bmSomeFunction)->Unit(benchmark::kMillisecond)->Iterations(100)->Repetitions(10);
// Run the benchmark
BENCHMARK_MAIN();



//int main(const int argc, const char *argv[]) {
//
// Inputs *In = new Inputs();
// Stats *St = new Stats();
// Outputs *Out = new Outputs();
// auto TB = new TestingBench(In, Out, St);
// TB->run();
//
// std::cout<<"-----------------"<<std::endl;
// auto T1 = new Test1(In, Out, St);
// T1->run();
// return 0;
//}