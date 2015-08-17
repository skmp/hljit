namespace hljit {

	namespace impl {
		template<typename T> struct remove_class_ref { };
		template<typename C, typename R, typename... A>
		struct remove_class_ref<R(C::*)(A...)> { using type = R(*&)(A...); };
		template<typename C, typename R, typename... A>
		struct remove_class_ref<R(C::*)(A...) const> { using type = R(*&)(A...); };
		template<typename C, typename R, typename... A>
		struct remove_class_ref<R(C::*)(A...) volatile> { using type = R(*&)(A...); };
		template<typename C, typename R, typename... A>
		struct remove_class_ref<R(C::*)(A...) const volatile> { using type = R(*&)(A...); };

		template<typename T> struct remove_class { };
		template<typename C, typename R, typename... A>
		struct remove_class<R(C::*)(A...)> { using type = R(*)(A...); };
		template<typename C, typename R, typename... A>
		struct remove_class<R(C::*)(A...) const> { using type = R(*)(A...); };
		template<typename C, typename R, typename... A>
		struct remove_class<R(C::*)(A...) volatile> { using type = R(*)(A...); };
		template<typename C, typename R, typename... A>
		struct remove_class<R(C::*)(A...) const volatile> { using type = R(*)(A...); };

		template <class T1, class ...T>
		struct first
		{
			typedef T1 type;
		};

		template <class T1, class ...T>
		struct last
		{
			typedef typename last<T...>::type type;
		};

		template <class T1>
		struct last<T1>
		{
			typedef T1 type;
		};

		template <class... A>
		struct fun_arglist;

		template <class A0, class... A>
		struct list_skip_first {
			typedef fun_arglist<A...> type;
		};

		template <class... A>
		struct fun_arglist {
			typedef typename first<A...>::type first;
			typedef typename list_skip_first<A...>::type skip_first;
			typedef typename last<A...>::type last;

			template <class T>
			using fun_t = T(*)(A...);
		};

		template<class... A>
		struct merge_arglist;

		template <class... A, class... B>
		struct merge_arglist<fun_arglist<A...>, B...> {
			typedef fun_arglist<A..., B...> type;
		};

		template <class A, class... B>
		struct merge_arglist<A, B...> {
			typedef fun_arglist<A, B...> type;
		};

		template <class T>
		struct fun_type {

		};

		template <class R, class... A>
		struct fun_type<typename R(*)(A...)> {
			typedef R rv;
			typedef fun_arglist<A...> arglist;
		};

		template <class T>
		struct unvar {
			typedef T type;
		};

		template <class T>
		struct unvar<var<T>> {
			typedef T type;
		};

		template <class T>
		struct unvar_always; //TODO: static assert here
		

		template <class T>
		struct unvar_always<var<T>> {
			typedef T type;
		};

		template <>
		struct unvar_always<void> {
			typedef var<void> type;
		};

		template <class L, class... A>
		struct unvar_list_ {
			
		};

		template <class L, class T, class... A>
		struct unvar_list_<fun_arglist<L>, T, A...> {
			typedef typename unvar_list_<merge_arglist<L, typename unvar_always<T>::type>, A...>::type type;
		};

		template <class L, class T>
		struct unvar_list_<fun_arglist<L>, T> {
			typedef typename merge_arglist<L, typename unvar_always<T>::type>::type type;
		};

		template <class... A>
		struct unvar_list {
			typedef typename fun_arglist<> type;
		};

		template <class T, class... A>
		struct unvar_list<T, A...> {
			typedef typename unvar_list_<fun_arglist<typename unvar_always<T>::type>, A...>::type type;
		};

		template <class T>
		struct unvar_list<T> {
			typedef typename fun_arglist<typename unvar_always<T>::type> type;
		};

		template <class R, class A>
		struct declFn;

		template <class R, class... A>
		struct declFn<R, fun_arglist<A...>> {
			using type = R(*)(A...);
		};


		template <class T>
		struct fun_unvar_type {

		};

		template <class R, class... A>
		struct fun_unvar_type<typename R(*)(A...)> {
			typedef typename unvar_always<R>::type rv;
			typedef typename unvar_list<A...>::type arglist;

			//typedef typename arglist::fun_t<typename rv> fun_t;
			typedef typename declFn<rv, arglist>::type fun_t;
		};

		template <bool f, class T>
		struct selector {

		};

		template <class T>
		struct selector<false, T> {

		};

		template<class C>
		struct lambda_type {
			typedef typename remove_class<decltype(&C::operator())>::type fun_t;
			typedef typename remove_class_ref<decltype(&C::operator())>::type fun_ref_t;
			
			typedef typename fun_type<fun_t>::arglist arglist;

			typedef typename fun_type<fun_t>::rv rv_t;

			typedef fun_t complex_type;
			static const bool is_static = std::is_assignable<fun_ref_t, C>::value;
		};

		template<class C>
		struct lambda_unvar {
			typedef typename remove_class<decltype(&C::operator())>::type fun_t;

			typedef typename fun_unvar_type<fun_t> unvar;

			typedef typename unvar::fun_t fun_unvar_t;
			typedef typename unvar::arglist arglist;
			typedef typename unvar::rv rv;
		};

	}
}