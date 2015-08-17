namespace hljit {

	struct else_expr {
		/*
		template<typename T>
		else_expr _else_if(cond cc, T fn) {
		set = true;
		e->L(lbl2);
		}

		template<typename T>
		else_expr _else(T fn) {
		set = true;
		e->L(lbl2);
		}
		*/
		~else_expr() {
			
		}
	};

	template<typename T>
	void /*else_expr*/ _if(cond cc, T fn) {
		else_expr rv;
		
		fn();

		//return rv;
	}

	template<typename T>
	void _while(cond cc, T fn) {
		fn();
	}

	template<typename T>
	void _unroll(int times, T fn) {
		for (int i = 0; i < times; i++)
			fn();
	}

	struct do_expr {
		
		template<typename T>
		void _while(T blk) {
			blk();
		}

		template<typename T>
		void _until(T blk) {
			blk();
		}
	};

	template<typename T>
	do_expr _do(T fn) {
		do_expr rv;
		
		fn();

		return rv;
	}
}