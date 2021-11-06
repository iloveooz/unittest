#include <string>
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <exception>

using Synonyms = std::map<std::string, std::set<std::string>>;

void AddSynonyms(Synonyms& synonyms, const std::string& first_word, const std::string& second_word) {
	synonyms[second_word].insert(first_word);
	synonyms[first_word].insert(second_word);
}

size_t GetSynonymsCount(Synonyms& synonyms, const std::string& first_word) {
	return synonyms[first_word].size();
}

bool AreSynonyms(Synonyms& synonyms, const std::string& first_word, const std::string& second_word) {
	return synonyms[first_word].count(second_word) == 1;
}

template <class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint) {
	if (t != u) {
		std::ostringstream os;
		os << "AssertEqual failed: " << t << " != " << u << ". Hint: " << hint;
		throw std::runtime_error(os.str());
	}
}

void Assert(bool value, const std::string& hint) {
	AssertEqual(value, true, hint);
}

void TestAddSynonyms() {
	{
		Synonyms empty;
		AddSynonyms(empty, "a", "b"); // a u b теперь синонимы
		const Synonyms expected = {
			{"a", {"b"}}, {"b", {"a"}}
		};
		Assert(empty == expected, "empty == expected");
	}
	{
		Synonyms synonyms = {
			{"a", {"b"}},
			{"b", {"a", "c"}},
			{"c", {"b"}}
		};
		AddSynonyms(synonyms, "a", "c");

		Synonyms expected = {
			{"a", {"b", "c"}},
			{"b", {"a", "c"}},
			{"c", {"b", "a"}}
		};
		Assert(synonyms == expected, "synonyms == expected");
	}
}

void TestCount() {
	{
		Synonyms empty;
		AssertEqual(GetSynonymsCount(empty, "a"), 0, "count for empty");
	}
	{
		Synonyms synonyms = {
			{"a", {"b", "c"}},
			{"b", {"a"}},
			{"c", {"a"}}
		};
		AssertEqual(GetSynonymsCount(synonyms, "a"), 2, "count for a");
		AssertEqual(GetSynonymsCount(synonyms, "b"), 1, "count for b");
		AssertEqual(GetSynonymsCount(synonyms, "z"), 0, "count for z");
	}
}

void TestAreSynonyms() {
	{
		Synonyms empty;
		Assert(!(AreSynonyms(empty, "a", "b")), "empty a b");
		Assert(!(AreSynonyms(empty, "b", "a")), "empty b a");
	}
	{
		Synonyms synonyms = {
			{"a", {"b", "c"}},
			{"b", {"a"}},
			{"c", {"a"}}
		};
		Assert(AreSynonyms(synonyms, "a", "b"), "syn a b");
		Assert(AreSynonyms(synonyms, "b", "a"), "syn b a");

		Assert(AreSynonyms(synonyms, "a", "c"), "syn a c");
		Assert(AreSynonyms(synonyms, "c", "a"), "syn c a");

		Assert(!AreSynonyms(synonyms, "b", "c"), "syn b c");
		Assert(!AreSynonyms(synonyms, "c", "b"), "syn c b");
	}
}

class TestRunner {
public:
	~TestRunner() {
		if (fail_count > 0) {
			std::cerr << fail_count << " tests failed. Terminate!" << std::endl;
			exit(1);
		}
	}

	template <class TestFunc>
	void RunTest(TestFunc function, const std::string& fname) {
		try {
			function();
			std::cerr << fname << " OK" << std::endl;
		} catch (std::runtime_error& e) {
			++fail_count;
			std::cerr << fname << " fail: " << e.what() << std::endl;
		}
	}
private:
	int fail_count = 0;
};

void testAll() {
	TestRunner tr;
	tr.RunTest(TestCount, "TestCount");
	tr.RunTest(TestAreSynonyms, "TestAreSynonyms");
	tr.RunTest(TestAddSynonyms, "TestAddSynonyms");
}

int main() {
	testAll();

	int q = 0;
	std::cin >> q;

	Synonyms synonyms;

	for (int i = 0; i < q; i++) {
		std::string operation_code;
		std::cin >> operation_code;

		if (operation_code == "ADD") {
			std::string first_word, second_word;
			std::cin >> first_word >> second_word;

			AddSynonyms(synonyms, first_word, second_word);
		} else if (operation_code == "COUNT") {
			std::string word;
			std::cin >> word;

			std::cout << GetSynonymsCount(synonyms, word) << std::endl;
		} else if (operation_code == "CHECK") {
			std::string first_word, second_word;
			std::cin >> first_word >> second_word;

			if (AreSynonyms(synonyms, first_word, second_word)) {
				std::cout << "YES" << std::endl;
			} else {
				std::cout << "NO" << std::endl;
			}
		}
	}

	return 0;
}
