#include <concepts>
#include <utility>
template<typename Advice, typename... Args>
concept HasMemberBefore = requires (Advice advice) {
	advice.before(std::declval<Args>()...);
};

template<typename Advice, typename... Args>
concept HasMemberAfter = requires (Advice advice) {
	advice.after(std::declval<Args>()...);
};

class noncopyable {
protected:
	noncopyable() = default;
	~noncopyable() = default;
	noncopyable(const noncopyable&) = delete;
	const noncopyable& operator=(const noncopyable&) = delete;
};

template<typename Func, typename... Args>
class aspect final : noncopyable {
public:
	aspect(Func&& func) : func_(std::forward<Func>(func)) {}

	template<typename... Advice>
	void invoke(Args&&... args, Advice&&... advice) const {
		auto invoke_before = [&](auto&& ad) {
			if constexpr (HasMemberBefore<decltype(ad), Args...>)
				ad.before(std::forward<Args>(args)...);
		};
	
		auto invoke_after = [&](auto&& ad) {
			if constexpr (HasMemberAfter<decltype(ad), Args...>)
				ad.after(std::forward<Args>(args)...);
		};
	
		int dummy;
		(invoke_before(advice), ...);
		func_(std::forward<Args>(args)...);
		//dummy和逗号表达式的存在是为了逆序执行after语句
		(dummy = ... = (invoke_after(advice), 0));
	}


private:
	Func func_;
};

template<typename... Advice, typename... Args, typename Func>
void make_aspect(Func&& func, Args&&... args) {
	aspect<Func, Args...> ap(std::forward<Func>(func));
	ap.invoke(std::forward<Args>(args)..., Advice()...);
}
