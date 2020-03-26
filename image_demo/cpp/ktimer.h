#ifndef K_TIMER_H_
#define K_TIMER_H_

#include <thread>
#include <chrono>
#include <string>
#include <ctime>

/**
about time funtions
	
	sleep

*/

namespace kipp{

	using kclock = std::chrono::time_point<std::chrono::system_clock>;
	using kns = std::chrono::nanoseconds;
	using kus = std::chrono::microseconds;
	using kms = std::chrono::milliseconds;
	using kseconds = std::chrono::seconds;
	using kminutes = std::chrono::minutes;
	using khours = std::chrono::hours;

	inline kclock know(){return std::chrono::system_clock::now();}
	
	inline std::time_t to_time_t(kclock t) {return std::chrono::system_clock::to_time_t(t);}
	
	inline kclock from_time_t(std::time_t t) {return std::chrono::system_clock::from_time_t(t);}
	
	inline std::string kget_time(std::time_t t/*std::time(nullptr)*/, const std::string& fmt/*"%d.%m.%Y %H:%M:%S"*/, bool is_local=true) {
		char mbstr[256];
		std::strftime(mbstr, sizeof(mbstr), fmt.c_str(), is_local ? std::localtime(&t) : std::gmtime(&t));
		return mbstr;
	}
	
	inline std::string kget_time(kclock t , const std::string& fmt, bool is_local=true) {

		return kget_time(to_time_t(t), fmt, is_local);
	}
	
	inline std::string kget_time(kclock t){std::time_t tt = to_time_t(t); return std::ctime(&tt);}
	
	template<typename T = kms>
	inline void ksleep(int value) {std::this_thread::sleep_for(T(value)); }
	
	template<typename T = kms>
	inline int kduration(kclock start, kclock end){
		return int(std::chrono::duration_cast<T>(end-start).count());
	}
	
	template<typename T = kms>
	inline std::string kstamp(){
		return std::to_string(std::chrono::duration_cast<T>(\
					know().time_since_epoch()).count());}
	

	class StopWatch final{
	private:
		kclock begin_, end_;
	public:
		StopWatch():begin_(know()), end_(begin_){}
		~StopWatch(){}

		inline void restart(){  reset(know()); }
		inline void stop() { end_ = know(); }
		inline void reset(kclock x) { begin_ = end_ = x; }

		inline kclock &begin() { return begin_; }
		inline kclock &end() { return end_; }

		template<typename T = kms>
		inline int elapsed(bool do_stop=false){
			kclock end = know();
			return kduration<T>(begin_, do_stop?(end_ = end):end);
		}
		template<typename T = kms>
		inline int result() { return kduration<T>(begin_, end_); }


	};

}//namespace kipp

#endif//ktimer.h