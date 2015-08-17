namespace hljit {

	namespace impl {

		extern int var_names;
		extern int value_names;

		struct var_impl {
			int var_name = var_names++;
			int value_id = value_names++;
		};

	};

	template <typename T>
	struct var : public impl::var_impl {

		var<T> operator+ (int other) {
			var<T> rv;
			var<T> vthis = *this;

			impl::emit([=] {
				return gen_op_infered([](int s1, int s2) { return s1 + s2; }, *impl::get_storage(rv.value_id), *impl::get_storage(vthis.value_id), other);
			});

			return rv;
		}

		var<T> operator+ (var<int> other) {
			var<T> rv;
			var<T> vthis = *this;

			impl::emit([=] {
				return gen_op_infered([](int s1, int s2) { return s1 + s2; }, *impl::get_storage(rv.value_id), *impl::get_storage(vthis.value_id), *impl::get_storage(other.value_id));
			});

			return rv;
		}
	};

	struct cond {

	};
	//var<int> + var<int> -> op_add(i, i) -> int +<int>
}
