High Level Jit
--------------	
An idea for high level codegen. I got a bit carried away w/ a templating backend, though


Concept ideas
-------------
    def([](var<int> a) {
      var<int> b;
      _if ( a == 0, [] {
        b = 5;
      })._else([] {
        b = a-1;
      });

      return b;
    });

    def([](var<int> a) {
      _do([] {
        call([](int param) {
          printf("Counting %d\n", param);
        }, a);
      })._while( a > 0);
    });