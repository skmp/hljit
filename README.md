High Level Jit
--------------	
An idea for high level codegen. I got a bit carried away w/ a templating backend, though

`var<int>` and `var<float>` to store values, `cond` to store conditions, using lambdas to 
create scope / structured flow control. Calls inferred from their types w/ templates.

Maybe connect w/ llvm backend?

Concept ideas
-------------
```c++
auto code_a = def([&](var<int> a) {
  var<int> b;
  _if ( a == 0, [&] {
    b = 5;
  })._else([&] {
    b = a-1;
  });
  return b;
});

auto code_b = def([&](var<int> a) {
  _do([&] {
    call([&](int param) {
      printf("Counting %d\n", param);
    }, a);
  })._while( a > 0);
});

bool mute = true;

//mute is evaluated during compile time
auto code_c = def([&](var<int> a) {
  _do([&] {
    if (!mute) {
        call([&](int param) {
          printf("Counting %d\n", param);
        }, a);
    }
  })._while( a > 0);
});

code_a(4);
code_b(3);
code_c(5);
```
