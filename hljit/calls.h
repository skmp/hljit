namespace hljit {

	namespace impl {
		template <class DT, class T>
		struct callgen_simple {

		};

		template <>
		struct callgen_simple<int, int> {
			static void gen(int arg) {
				//		e->push((int&)arg);
				printf("passing int %d\n", arg);
			}
		};

		template <>
		struct callgen_simple<float, float> {
			static void gen(float arg) {
				//	e->push((int&)arg);
				printf("passing float %f\n", arg);
			}
		};

		template <>
		struct callgen_simple<int, var<int>> {
			static void gen(var<int> arg) {
				//e->push(e->eax);
				printf("passing var<int>\n");
			}
		};

		template <>
		struct callgen_simple<float, var<float>> {
			static void gen(var<float> arg) {
				//e->push(e->ecx);
				printf("passing var<float>\n");
			}
		};

		template<class FA, class... A>
		struct callgen {

		};

		template <class FA, class F, class... A>
		struct callgen<FA, F, A...> {
			static void gen(F first, A... args) {
				callgen_simple<FA::first, F>::gen(first);
				callgen<FA::skip_first, A...>::gen(args...);
			}
		};

		template <class FA, class F>
		struct callgen<FA, F> {
			static void gen(F first) {
				callgen_simple<FA::first, F>::gen(first);
			}
		};

		template<class FA>
		struct callgen<FA> {
			static void gen() {
				
			}
		};

		template <class T>
		struct callret {

		};

		template <>
		struct callret<int> {
			static void gen() {
				printf("Return: var<int>\n");
			}
		};

		template <>
		struct callret<float> {
			static void gen() {
				printf("Return: var<float>\n");
			}
		};

		template <>
		struct callret<void> {
			static void gen() {

			}
		};

		template <class F, class FR, class FA, class... A>
		var<FR> call_i(F fun, A... args) {
			printf("Generating call to %08X\n", fun);
			callgen<FA, A...>::gen(args...);
			callret<FR>::gen();

			return var<FR>();
		}
		
		/*
		template <class F, class FR, class FA, class... A>
		var<FR> call_lambda_i<true, F, FR, FA, A...>(F fun, A... args) {
			return call_i < lambda_type<F>::fun_t, FR, FA, A...>((lambda_type<F>::fun_t)fun, args...);
		}
		*/

		/*
		{
			printf("Generating call to %08X\n", fun);
			callgen<FA, A...>::gen(args...);
			callret<FR>::gen();

			return var<FR>();
		}  */

		template <class T, class... A>
		struct call_i2 {
			typedef var<typename impl::lambda_type<T>::rv_t> rv;
			typedef typename lambda_type<T>::fun_t fun_t;

			//using a function here confuses msvc2013
			template <bool is_static>
			struct static_resolver {
				static rv gen(T closure, A... args) {
					return call_i2<fun_t, A...>::gen(closure, args...);
				}
			};
			
			template<>
			struct static_resolver<false> {
				static rv gen(T closure, A... args) {
					printf("Lambda Context is %p\n", (void*)&closure);

					auto fun_pointer = &T::operator();

					return call_i2<fun_t, A...>::gen((fun_t&)fun_pointer, args...);
				}
			};
			

			static rv gen(T closure, A... args) {
				return static_resolver<lambda_type<T>::is_static>::gen(closure, args...);
			}
		};

		template <class FR, class... FA, class... A>
		struct call_i2<FR(*)(FA...), A...> {
			using F = FR(*)(FA...);
			typedef var<typename impl::fun_type<F>::rv> rv;

			static rv gen(F function, A... args) {
				return call_i<F, FR, fun_type<F>::arglist, A...>(function, args...);
			}
		};

		template <class C, class FR, class... FA, class E, class... A>
		struct call_i2<FR(C::*)(FA...), E, A...> {
			using F = FR(*)(FA...);
			using FC = FR(C::*)(FA...);
			typedef var<typename impl::fun_type<F>::rv> rv;

			static rv gen(FC function, C* context, A... args) {
				//TODO: Make sure E == C at this point
				printf("Class Context is %p\n", (void*)context);

				auto fun_pointer = function;

				return call_i2<F, A...>::gen((F&)fun_pointer, args...);
			}
		};
	}


	//const lambda_type<F>::is_static
	template <class F, class... A>
	typename impl::call_i2<F, A...>::rv call(F fun, A... args) {
		return impl::call_i2<F, A...>::gen(fun, args...);
		//return impl::call_lambda_i<true, F, typename impl::lambda_type<F>::rv_t, typename impl::lambda_type<F>::arglist, A...>::gen(fun, args...);
	};

	//template <class FR, class... FA, class... A>
	//var<typename impl::fun_type<decltype(FR(*)(FA...))>::rv> call(decltype(FR(*)(FA...))fun, A... args) {
	//	return impl::call_i<decltype(FR(*)(FA...)), typename impl::fun_type<decltype(FR(*)(FA...))>::rv, typename impl::fun_type<decltype(FR(*)(FA...))>::arglist, A...>(fun, args...);
	//};
	/*
	template <class FR, class... FA, class... A>
	var<typename impl::fun_type<decltype(FR(*)(FA...))>::rv> call(decltype(FR(*)(FA...))fun, A... args) {
		return impl::call_i<decltype(FR(*)(FA...)), typename impl::fun_type<decltype(FR(*)(FA...))>::rv, typename impl::fun_type<decltype(FR(*)(FA...))>::arglist, A...>(fun, args...);
	};
	*/

}