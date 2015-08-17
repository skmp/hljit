namespace hljit {
	
	struct op_base {
		virtual void exec() = 0;
	};


	template <typename C>
	struct op_infered : op_base {
		C ctx;

		op_infered(const C& _ctx): ctx(_ctx) {
			
		}

		virtual void exec (){
			ctx();
		}
	};


	template<typename C, typename R, typename... A>
	//void gen_op_infered( R(*function)(A... args), R* d, A*... args) {
	op_base* gen_op_infered(C lambda, R& d, A&... args) {
		typename impl::declFn<R, typename impl::fun_arglist< A... >>::type function = lambda;

		auto dispatcher = [&]() mutable { d = lambda(args...); };

		return new op_infered<decltype(dispatcher)>(dispatcher);
	}
}