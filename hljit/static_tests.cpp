#include "hljit.h"

namespace hljit {
	namespace impl {

		namespace {
			int test_int(int v) {
				printf("%d\n", v);
				return 5;
			}

			int test_noargs() {
				printf("%d\n", 2);
				return 4;
			}

			void test_norv() {
				printf("%d\n", 1);
			}

			void sanity_checks() {

				auto x = []{ return 5; };
				auto y = [x]{ return x(); };

				static_assert(lambda_type<decltype(x)>::is_static, "non-catpure should convert");
				static_assert(!lambda_type<decltype(y)>::is_static, "catpure should not convert");


				//CALL FUNCTION
				{
					var<int> rv = hljit::call(test_int, 5);
				}

				{
					var<int> rv = hljit::call(test_noargs);
				}

				{
					var<void> rv = hljit::call(test_norv);
				}

				//CALL LAMBDA NON-CAPTURE
				{
					var<void> rv = hljit::call([]() {

					});
				}

				{
					var<int> rv = hljit::call([]() {
						return 3;
					});
				}

				{
					var<int> rv = hljit::call([](int i, int b) {
						return 3 + i + b;
					}, 1, 3);
				}

				//CALL LAMBDA CAPTURE
				{
					int a = 5;
					var<void> rv = hljit::call([a]() {

					});
				}

				{
					int a = 5;
					var<int> rv = hljit::call([a]() {
						return 3 + a;
					});
				}

				{
					int a = 5;
					var<int> rv = hljit::call([a](int i, int b) {
						return 3 + i + b + a;
					}, 1, 3);
				}

				//Class Member functions
				{
					struct test {
						int val;

						void method_vv() {
							printf("%d\n", val);
						}

						int method_ii(int param) {
							printf("%d\n", val + param);

							return val + param;
						}

						int method_iii(int param, int param2) {
							printf("%d\n", val + param + param2);

							return val + param;
						}
					};

					auto inst = new test();

					inst->val = 5;

					{
						var<void> rv = hljit::call(&test::method_vv, inst);
					}

					{
						var<int> rv = hljit::call(&test::method_ii, inst, 1);
					}

					{
						var<int> rv = hljit::call(&test::method_iii, inst, 1, 3);
					}
				}

				//DEFS
#if FIXME_REAL_COMPILE
				{
					void(*rv)(int, float) = def([](var<int>, var<float>) {

					});
				}


				{
					bool(*rv)(int, float) = def([](var<int>, var<float>) {
						return var<bool>();
					});
				}


				{
					bool(*rv)(int, float) = def([](var<int>, var<float>) {
						return var<bool>();
					});
				}
#else
				//TODO: Void functions are broken

				{
					std::function<int()> rv = def([]() {
						return var<int>();
					});
				}

				{
					std::function<int(int)> rv = def([](var<int>) {
						return var<int>();
					});
				}


				{
					std::function<int(int, float)> rv = def([](var<int>, var<float>) {
						return var<int>();
					});
				}


				{
					std::function<int(int, float)> rv = def([](var<int>, var<float>) {
						return var<int>();
					});
				}
#endif
			}
		}
	}

	void run_tests() {
		impl::sanity_checks();
	}
}