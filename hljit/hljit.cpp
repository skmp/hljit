#include "hljit.h"

namespace hljit {
	namespace impl {
		int var_names = 1;
		int value_names = 1;

		static union {
			float f;
			int i;
		} values[1024];

		block* current;

		void emit(std::function<op_base*()> emitter) {
			current->ops.push_back(emitter);
		}

		int* get_storage(int value) {
			return &values[value].i;
		}

		void start_block() {
			current = new block();
		}

		void end_block() {
			//something?
		}
	}
}