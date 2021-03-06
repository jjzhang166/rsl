#include <UnitTest++/UnitTest++.h>

#define RSL_NULL_PTR_POLICY_DEFAULT(T) rsl::track::may_be_null<T>
#define RSL_ON_DANGLE_POLICY_DEFAULT(T) rsl::track::null_on_dangle<T>

#include <track/vector.h>

struct ComplexObject
{
	int i = 0;
	std::string s = "test";
};

SUITE(vector_tests)
{
	using namespace rsl;
	using namespace rsl::track;

	TEST(index_vector_test)
	{
		vector<int> a{3, 3, 3};

		auto ptr = get_ptr_at(a, 0);
		CHECK_EQUAL(3, *ptr);
		*ptr = 5;
		CHECK_EQUAL(5, a[0]);

		auto cptr = cget_ptr_at(a, 2);
		a[2] = 7;
		CHECK_EQUAL(7, *cptr);
	}

	TEST(iterator_vector_test)
	{
		vector<std::string> a;
		a.assign(3, "hello");

		auto ptr = get_ptr(a, a.begin());
		CHECK_EQUAL("hello", *ptr);
		*ptr = "world";
		CHECK_EQUAL("world", a[0]);

		auto cptr = cget_ptr(a, a.cbegin() + 2);
		a[2] = "!";
		CHECK_EQUAL("!", *cptr);
	}

	TEST(delete_vector_test)
	{
		auto a = std::make_unique<vector<std::vector<int>>>();
		a->resize(3);

		auto ptr1 = get_ptr_at(*a, 0);
		auto ptr2 = cget_ptr_at(*a, 1);
		auto ptr3 = get_ptr(*a, a->begin());
		auto ptr4 = cget_ptr(*a, a->cbegin() + 2);

		CHECK(ptr1);
		CHECK(ptr2);
		CHECK(ptr3);
		CHECK(ptr4);

		a.reset();

		CHECK(!ptr1);
		CHECK(!ptr2);
		CHECK(!ptr3);
		CHECK(!ptr4);
	}

	TEST(clear_vector_test)
	{
		vector<char> a;
		a.resize(3);

		auto ptr1 = get_ptr_at(a, 0);
		auto ptr2 = cget_ptr_at(a, 1);
		auto ptr3 = get_ptr(a, a.begin());
		auto ptr4 = cget_ptr(a, a.cbegin() + 2);

		CHECK(ptr1);
		CHECK(ptr2);
		CHECK(ptr3);
		CHECK(ptr4);

		a.clear();

		CHECK(!ptr1);
		CHECK(!ptr2);
		CHECK(!ptr3);
		CHECK(!ptr4);
	}

	TEST(erase_vector_test)
	{
		vector<std::pair<int, std::string>> a;
		a.resize(3);

		auto ptr0 = cget_ptr(a, a.cbegin() + 0);
		auto ptr1 = cget_ptr(a, a.cbegin() + 1);
		auto ptr2 = cget_ptr(a, a.cbegin() + 2);

		CHECK(ptr0);
		CHECK(ptr1);
		CHECK(ptr2);

		a.erase(a.cbegin() + 1);

		CHECK(ptr0);
		CHECK(ptr1);
		CHECK(!ptr2);

		a.erase(a.cbegin() + 1);

		CHECK(ptr0);
		CHECK(!ptr1);
		CHECK(!ptr2);

		a.erase(a.cbegin());

		CHECK(!ptr0);
		CHECK(!ptr1);
		CHECK(!ptr2);
	}

	TEST(insert_vector_test)
	{
		vector<int> a;
		a.reserve(6);
		a.push_back(0);
		a.push_back(0);
		a.push_back(0);

		CHECK_EQUAL(6u, a.capacity());

		auto ptr0 = cget_ptr(a, a.cbegin() + 0);
		auto ptr1 = cget_ptr(a, a.cbegin() + 1);
		auto ptr2 = cget_ptr(a, a.cbegin() + 2);

		CHECK_EQUAL(0, *ptr0);
		CHECK_EQUAL(0, *ptr1);
		CHECK_EQUAL(0, *ptr2);

		a.insert(a.begin(), 2);

		CHECK_EQUAL(2, *ptr0);
		CHECK_EQUAL(0, *ptr1);
		CHECK_EQUAL(0, *ptr2);

		a.insert(a.begin(), -1);

		CHECK_EQUAL(-1, *ptr0);
		CHECK_EQUAL(2, *ptr1);
		CHECK_EQUAL(0, *ptr2);

		a.insert(a.begin() + 1, 5);

		CHECK_EQUAL(-1, *ptr0);
		CHECK_EQUAL(5, *ptr1);
		CHECK_EQUAL(2, *ptr2);

		a.insert(a.begin(), 8);

		CHECK(!ptr0);
		CHECK(!ptr1);
		CHECK(!ptr2);
	}

	TEST(complex_object_vector_test)
	{
		pointer<ComplexObject> ptr1;
		pointer<int> ptr2;
		pointer<std::string> ptr3;

		{
			vector<ComplexObject> a;
			a.resize(3);

			ptr1 = get_ptr_at(a, 1);
			ptr2 = make_ptr(&ptr1->i, ptr1);
			ptr3 = make_ptr(&ptr1->s, ptr2);

			CHECK(ptr1);
			CHECK(ptr2);
			CHECK(ptr3);

			ptr1->i = 100;
			ptr1->s = "hello";
			CHECK_EQUAL(100, *ptr2);
			CHECK_EQUAL("hello", *ptr3);

			*ptr2 = 50;
			CHECK_EQUAL(50, ptr1->i);

			*ptr3 = "world";
			CHECK_EQUAL("world", ptr1->s);
		}

		CHECK(!ptr1);
		CHECK(!ptr2);
		CHECK(!ptr3);
	}
}
