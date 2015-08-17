namespace hljit {
	namespace impl {
		struct block {
			std::vector<std::function<op_base*()>> ops;
		};

		extern block* current;

		template <class T>
		static T returnType(var<T> v) {
			return *(T*)impl::get_storage(v.value_id);
		}

		template<>
		static void returnType<void>(var<void> v) {

		}

		template <class C, class R, class F, class L>
		struct compiled_block;
		
		template <class C, class R, class F, class... A>
		struct compiled_block <C, R, F, impl::fun_arglist<A...>> {
			using rv_t = std::function<R(A...)>;

			std::vector<op_base*> ops;

			

			rv_t gen(var<R> out, var<A>... args_v) {
				for (size_t i = 0; i < current->ops.size(); i++) {
					ops.push_back(current->ops[i]());
				}

				return [=](A... args) -> R {
					int* arn[sizeof...(A)+1] = { (int*)&args... };
					int* aron[sizeof...(A)+1] = { 1 + (int*)&args_v... };

					for (size_t i = 0; i < sizeof...(A); i++) {
						*(int*)impl::get_storage(*aron[i]) = *arn[i];
					}

					for (size_t i = 0; i < ops.size(); i++) {
						ops[i]->exec();
					}

					//u32 ofs[] = { args_v... };
					//impl::get_storage(args_v)... = args...;
					return returnType(out);
				};
			}
		};
	}
}