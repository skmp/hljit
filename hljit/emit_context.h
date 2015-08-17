namespace hljit {
	namespace impl {
		void emit(std::function<op_base*()> emitter);
		int* get_storage(int value);

		void start_block();

		void end_block();
	}
}