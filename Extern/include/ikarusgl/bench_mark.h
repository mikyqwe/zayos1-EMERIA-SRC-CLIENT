#pragma once
#ifndef __INCLUDE_HEADER_BENCHMARK_IKARUSGL__
#define __INCLUDE_HEADER_BENCHMARK_IKARUSGL__

namespace ikgl
{
	/**
	* Enumeration used as return value for the function/lambda to use in a for_range argument
	*/
	enum class for_range_return {
		_break = false, /**< value used to interrupt the iteration*/
		_next = true,   /**< value used to continue the iteration*/
	};


	/**
	* Function used to iterate the iteration function for each value within the range
	* @param limit1 is used to define the start value of the range
	* @param limit2 is used to define the end value of the range (not included)
	* @param func this is the iteration function used to iterate each value within the range. The return of the function can interrupt the iteration or make it go on. The return is one of these values in the enumeration #for_range_return
	*/
	template <class T>
	void_t for_range(const T limit1, const T limit2, std::function<for_range_return(const T)> func) {
		if (limit1 > limit2) {
			for (T i = limit1; i > limit2; --i) {
				if (func(i) == for_range_return::_break)
					break;
			}
		} else {
			for (T i = limit1; i < limit2; ++i) {
				if (func(i) == for_range_return::_break)
					break;
			}
		}
	}
	
	/** Usefull features used to test the performance of the library's code*/
	namespace bench_marck 
	{
		class rating /**< class used to count the execution times of one or more functions / lambda and obtain the total execution time*/
		{
		public:
			rating() {
				this->_total_time = 0.0;
			}


			/**
			* Function used to trace the execution time of the function passed as argument. The execution time will add to the total amount that you can know by using get_execution_time() .
			* @param func is the function (or lambda) that it will executed by tracing the execution time
			* \return this function returns the execution time in seconds.
			*/
			template< class T>
			double_t execute(std::function<T()> func) {
				const auto start_time = utilities::get_time_double();
				func();
				const auto end_time = utilities::get_time_double();
				const auto ex_time = end_time - start_time;

				this->_total_time += ex_time;
				return ex_time;
			}


			/**
			* Function used to trace the execution time of the function passed as argument executed one or more times. The execution time will add to the total amount that you can know by using get_execution_time() .
			* @param func is the function (or lambda) that it will executed by tracing the execution time
			* @param times is the amout of times to execute the function/lambda
			* \return this function returns the execution time in seconds.
			*/
			template< class T>
			double_t execute(std::function<T()> func, dword_t times) {
				const auto start_time = utilities::get_time_double();
				for_range(0, times, [&func](const dword_t)->void_t {func(); });
				const auto end_time = utilities::get_time_double();
				const auto ex_time = end_time - start_time;

				this->_total_time += ex_time;
				return ex_time;
			}


			/**
			* Function used to get the total execution time.
			* \return this function returns the total execution time in seconds.
			*/
			double_t get_execution_time() {
				return this->_total_time;
			}

		private:
			double_t _total_time;
		};


		/**
		* Function used to trace the execution time of the function passed as argument executed one or more times. 
		* @param func is the function (or lambda) that it will executed by tracing the execution time
		* @param times is the amout of times to execute the function/lambda
		* \return this function returns the execution time in seconds.
		*/
		template< class T>
		double_t executing_time(std::function<T()> func, dword_t times=1) {
			const auto start_time = utilities::get_time_double();

			if (times == 1) {
				func();
			} else {
				for_range(0, times, [&func](const dword_t)->void_t {func(); });
			}

			const auto end_time = utilities::get_time_double();
			const auto ex_time = end_time - start_time;
			return ex_time;
		}
	}
}

#endif