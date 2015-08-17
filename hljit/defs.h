namespace hljit {

	template<class T>
	struct envoid{
		static var<T> doit(var<T> v) {
			return v;
		}
	};

	template<class T, class R, class L>
	struct call_args;

	template<class T, class R, class... A>
	struct call_args<T, R, impl::fun_arglist<A...>> {
		using rv_fun = std::function<R(A...)>;
		
		static rv_fun gen2(T fn, var<A>... args) {
			var<R> out = fn(args...);
			impl::end_block();
			auto g = new impl::compiled_block<T, impl::lambda_unvar<T>::rv, impl::lambda_unvar<T>::fun_unvar_t, impl::lambda_unvar<T>::arglist>();

			return g->gen(out, args...);
		}

		static rv_fun gen(T fn) {
			impl::start_block();

			//auto rv = call_args<T, impl::lambda_type<T>::rv_t, impl::lambda_type<T>::arglist>::gen(fn);
			return gen2(fn, var<A>()...);
		}
	};

	//typename impl::lambda_unvar<T>::fun_unvar_t def(T fn) {

	template<typename T>
	typename call_args<T, typename impl::lambda_unvar<T>::rv, typename impl::lambda_unvar<T>::arglist>::rv_fun def(T fn) {
		return call_args<T, impl::lambda_unvar<T>::rv, impl::lambda_unvar<T>::arglist>::gen(fn);
	}
}