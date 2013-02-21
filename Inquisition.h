//
//  Inquisition.h
//
//  Created by Arthur Langereis
//  Copyright (c) 2013 logicdream. All rights reserved.
//

#ifndef INQUISITION_INCLUDED
#define INQUISITION_INCLUDED

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>


namespace Inquisition {

	//  _____         _      ___  _     _           _
	// |_   _|__  ___| |_   / _ \| |__ (_) ___  ___| |_ ___
	//   | |/ _ \/ __| __| | | | | '_ \| |/ _ \/ __| __/ __|
	//   | |  __/\__ \ |_  | |_| | |_) | |  __/ (__| |_\__ \
	//   |_|\___||___/\__|  \___/|_.__// |\___|\___|\__|___/
	//                               |__/

	class TestCase;
	class TestRun;

	using TestMethod = std::function<void(TestRun &)>;
	
	
	class BasicTest {
		std::string name_;
		
	public:
		BasicTest(std::string name) : name_(name) {}
		virtual ~BasicTest() {}

		virtual void operator()(TestRun & res) = 0;
		
		const std::string & name() const { return name_; }
	};


	using TestPtr = std::shared_ptr<BasicTest>;
	using TestSet = std::vector<TestPtr>;
	using TestSetPtr = std::shared_ptr<TestSet>;
	

	class TestCase : public BasicTest {
		TestMethod method_;
		
	public:
		TestCase(std::string name, TestMethod method);
		void operator()(TestRun & res) override;
	};
	
	
	class TestGroup : public BasicTest {
		TestSetPtr subTests_;
		
	public:
		TestGroup(std::string name, const std::function<void()> & init);
		void operator()(TestRun & res) override;
	};


	//  _____         _     _____            _
	// |_   _|__  ___| |_  | ____|_   ____ _| |
	//   | |/ _ \/ __| __| |  _| \ \ / / _` | |
	//   | |  __/\__ \ |_  | |___ \ V / (_| | |
	//   |_|\___||___/\__| |_____| \_/ \__,_|_|
	//

	class TestResult {
		int passes_ = 0;
		int failures_ = 0;
		int errors_ = 0;
		const std::string label_;

	public:
		TestResult() {}
		TestResult(std::string label) : label_(label) {}

		void pass(const std::string & testName, const std::string & msg, const std::string & innerMsg = "");
		void failure(const std::string & testName, const std::string & msg, const std::string & innerMsg = "");
		void error(const std::string & testName, const std::string & msg, const std::string & innerMsg = "");
		
		int passes() const { return passes_; }
		int failures() const { return failures_; }
		int errors() const { return errors_; }
	};

	
	class TestRun {
		std::string label_;
		const TestSetPtr testSet_;
		TestSet::const_iterator curTest_;
		TestResult result_;
		std::vector<TestRun> subRuns_;
		int checkIndex_ = 0;
		
		void pass(const std::string & msg, const std::string & innerMsg = "");
		void failure(const std::string & msg, const std::string & innerMsg = "");
		void error(const std::string & msg, const std::string & innerMsg = "");

	public:
		TestRun(const std::string & label, const TestSetPtr & testSet);

		const std::string & label() const { return label_; }
		TestResult & result() { return result_; }
		void addSubRun(TestRun subRun);
		
		void run();
		
		// -- checks
		
		template <typename Expr>
		void checkImpl(Expr expr, const std::string & failMsg) {
			try {
				checkIndex_++;
				
				if (expr())
					pass("check #" + std::to_string(checkIndex_));
				else
					failure("check #" + std::to_string(checkIndex_), failMsg);
			}
			catch(const std::exception & ex) {
				failure("unexpected std::exception in check #" + std::to_string(checkIndex_), ex.what());
			}
			catch (const std::string & str) {
				failure("unexpected string exception in check #" + std::to_string(checkIndex_), str);
			}
			catch (const char * cp) {
				failure("unexpected c-string exception in check #" + std::to_string(checkIndex_), cp);
			}
			catch(...) {
				failure("unexpected other exception in check #" + std::to_string(checkIndex_));
			}
		}
		
		// -- checks
		
		void check_true(bool expr) { checkImpl([=] { return expr; }, "expression was expected to be true"); }
		void check_false(bool expr) { checkImpl([=] { return !expr; }, "expression was expected to be false"); }
		
		template <typename T, typename U>
		void check_equal(const T & t, const U & u) {
			checkImpl([=] { return t == u; }, std::to_string(t) + " was expected to be equal to " + std::to_string(u));
		}
		
		template <typename T, typename U>
		void check_not_equal(const T & t, const U & u) {
			checkImpl([=] { return t != u; }, std::to_string(t) + " was expected to differ from " + std::to_string(u));
		}
		
		template <typename T, typename U>
		void check_gt(const T & t, const U & u) {
			checkImpl([=] { return t > u; }, std::to_string(t) + " was expected to be greater than " + std::to_string(u));
		}
		
		template <typename T, typename U>
		void check_ge(const T & t, const U & u) {
			checkImpl([=] { return t >= u; }, std::to_string(t) + " was expected to be greater than or equal to " + std::to_string(u));
		}
		
		template <typename T, typename U>
		void check_lt(const T & t, const U & u) {
			checkImpl([=] { return t < u; }, std::to_string(t) + " was expected to be less than " + std::to_string(u));
		}
		
		template <typename T, typename U>
		void check_le(const T & t, const U & u) {
			checkImpl([=] { return t <= u; }, std::to_string(t) + " was expected to be less than or equal to " + std::to_string(u));
		}
	};

	
	//  _____ _      _
	// |  ___(_)_  _| |_ _   _ _ __ ___  ___
	// | |_  | \ \/ / __| | | | '__/ _ \/ __|
	// |  _| | |>  <| |_| |_| | | |  __/\__ \
	// |_|   |_/_/\_\\__|\__,_|_|  \___||___/
	//
	
	namespace detail {
		class FixtureBase {
		public:
			virtual ~FixtureBase() = default;
			
			static std::unordered_map<std::string, std::unique_ptr<FixtureBase>> fixtures_;
		};
		
		template <typename Fixture>
		class FixtureHolder : public FixtureBase {
			std::unique_ptr<Fixture> fixture_;
		public:
			FixtureHolder() : fixture_ { new Fixture() } {}
			Fixture & fixture() { return *fixture_.get(); }
		};
	}
	
	
	//  ____        __ _       _ _   _
	// |  _ \  ___ / _(_)_ __ (_) |_(_) ___  _ __
	// | | | |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \
	// | |_| |  __/  _| | | | | | |_| | (_) | | | |
	// |____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|
	//

	namespace detail {
		template <typename T, typename ...Args>
		void test(TestSetPtr set, Args&&... args) {
			set->push_back(std::unique_ptr<BasicTest>(new T(std::forward<Args>(args)...)));
		}
		
		template <typename T>
		void test(TestSetPtr set) {
			set->push_back(std::unique_ptr<BasicTest>(new T()));
		}
	}
	
	template <typename Fixture>
	void def_fixture(const std::string & name) {
		detail::FixtureBase::fixtures_.emplace(name, std::unique_ptr<detail::FixtureHolder<Fixture>>{ new detail::FixtureHolder<Fixture>() });
	}
	
	template <typename Fixture>
	Fixture & fixture(const std::string & name) {
		auto & fb = detail::FixtureBase::fixtures_.at(name);
		if (! fb)
			throw std::runtime_error("fb is null!");
		return dynamic_cast<detail::FixtureHolder<Fixture>*>(fb.get())->fixture();
	}

	void test(TestSetPtr set, const std::string & name, const TestMethod & method);
	void test(const std::string & name, const TestMethod & method);
	void group(const std::string & name, const std::function<void()> & init);

	void run(const TestSetPtr & test);
	void runAll();

} // namespace Inquisition


#endif /* defined(INQUISITION_INCLUDED) */
