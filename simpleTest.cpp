#include <cassert>
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
void Assertion(const T& t, const U& u, const std::string& hint) {
	if (t != u) {
		std::ostringstream os;
		os << "Assertion failed: " << t << " != " << u << ". Hint: " << hint;
		throw std::runtime_error(os.str());
	}
}

void TestAddSynonyms() {
	{
		Synonyms empty;
		AddSynonyms(empty, "a", "b"); // a u b теперь синонимы
		const Synonyms expected = {
			{"a", {"b"}}, {"b", {"a"}}
		};
		assert(empty == expected);
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
		assert(synonyms == expected);
	}
	std::cout << "TestAddSynonyms = OK" << std::endl;
}

void TestCount() {
	{
		Synonyms empty;
		Assertion(GetSynonymsCount(empty, "a"), 0, "count for empty");
	}
	{
		Synonyms synonyms = {
			{"a", {"b", "c"}},
			{"b", {"a"}},
			{"c", {"a"}}
		};
		Assertion(GetSynonymsCount(synonyms, "a"), 2, "count for a");
		Assertion(GetSynonymsCount(synonyms, "b"), 1, "count for b");
		Assertion(GetSynonymsCount(synonyms, "z"), 0, "count for z");
	}
	std::cout << "TestSynonymsCount = OK" << std::endl;
}

void TestAreSynonyms() {
	{
		Synonyms empty;
		assert(!(AreSynonyms(empty, "a", "b")));
		assert(!(AreSynonyms(empty, "b", "a")));
	}
	{
		Synonyms synonyms = {
			{"a", {"b", "c"}},
			{"b", {"a"}},
			{"c", {"a"}}
		};
		assert(AreSynonyms(synonyms, "a", "b"));
		assert(AreSynonyms(synonyms, "b", "a"));

		assert(AreSynonyms(synonyms, "a", "c"));
		assert(AreSynonyms(synonyms, "c", "a"));

		assert(!AreSynonyms(synonyms, "b", "c"));
		assert(!AreSynonyms(synonyms, "c", "b"));
	}
	std::cout << "TestAreSynonyms = OK" << std::endl;
}

void testAll() {
	TestCount();
	TestAreSynonyms();
	TestAddSynonyms();
}

int main() {
	testAll();

	return 0;

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

	system("pause");
	return 0;
}
