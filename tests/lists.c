#include "test_suite.h"
#include <adsc.h>

enum test_result_t list_create()
{
  ads_list_t* lst = ads_list_create(int);
  test_assert(lst != NULL);

  ads_list_destroy(&lst);
  test_assert(lst == NULL);

  return TEST_RESULT_OK;
}

enum test_result_t list_push_top_1()
{
  auto_t lst = ads_list_create(int*);
  int* ptr = malloc(sizeof(int));
  *ptr = 228;

  test_assert(ads_list_empty(lst));
  test_assert(lst->size == 0);

  ads_list_push(lst, &ptr);
  test_assert(ads_list_top(lst) != NULL);
  test_assert(lst->size == 1);
  test_assert(!ads_list_empty(lst));

  int** top = ads_list_top(lst);
  test_assert(**top == 228);
  test_assert(lst->size == 1);

  int* back = NULL;
  ads_list_pop(lst, &back);
  test_assert(lst->size == 0);
  test_assert(ads_list_top(lst) == NULL);
  test_assert(ads_list_empty(lst));
  test_assert(back == ptr);

  free(back);
  ads_list_destroy(&lst);

  return TEST_RESULT_OK;
}

enum test_result_t list_push_top_2()
{
  ads_list_t* lst = ads_list_create(int);
  int ptr;

  test_assert((int*) ads_list_top(lst) == NULL);
  test_assert(ads_list_empty(lst));

  ads_list_push_value(lst, 4);
  test_assert(*(int*) ads_list_top(lst) == 4);
  test_assert(!ads_list_empty(lst));

  ads_list_push_value(lst, 8);
  test_assert(*(int*) ads_list_top(lst) == 8);

  ads_list_push_value(lst, 7);
  test_assert(*(int*) ads_list_top(lst) == 7);

  ads_list_pop(lst, &ptr);
  test_assert(ptr == 7);
  test_assert(*(int*) ads_list_top(lst) == 8);

  ads_list_pop(lst, &ptr);
  test_assert(ptr == 8);
  test_assert(*(int*) ads_list_top(lst) == 4);

  ads_list_pop(lst, &ptr);
  test_assert(ptr == 4);
  test_assert((int*) ads_list_top(lst) == NULL);
  test_assert(ads_list_empty(lst));

  ads_list_push_value(lst, 4);
  test_assert(*(int*) ads_list_top(lst) == 4);

  ads_list_push_value(lst, 3);
  test_assert(*(int*) ads_list_top(lst) == 3);

  ads_list_push_value(lst, 2);
  test_assert(*(int*) ads_list_top(lst) == 2);

  test_assert(!ads_list_empty(lst));

  int i = 0;
  ads_list_for(lst, it)
  {
    int val = ads_list_val(it, int);

    switch (i)
    {
    case 0:
      test_assert(val == 2);
      break;
    case 1:
      test_assert(val == 3);
      break;
    case 2:
      test_assert(val == 4);
      break;
    default:
      test_assert(false);
    }

    i++;
  }

  ads_list_destroy(&lst);
  test_assert(lst == NULL);

  return TEST_RESULT_OK;
}

enum test_result_t list_for_value()
{
  ads_list_t* lst = ads_list_create(int);

  ads_list_push_value(lst, 10);
  ads_list_push_value(lst, 8);
  ads_list_push_value(lst, 6);
  ads_list_push_value(lst, 4);
  ads_list_push_value(lst, 2);
  ads_list_push_value(lst, 0);

  test_assert(lst->size == 6);
  int i = 0;

  ads_list_for(lst, it)
  {
    int val = ads_list_val(it, int);
    test_assert(val == i * 2);
    i++;
  }

  test_assert(i == 6);

  ads_list_clear(lst);
  test_assert(lst->size == 0);
  test_assert(ads_list_empty(lst));

  ads_list_destroy(&lst);

  return TEST_RESULT_OK;
}

enum test_result_t list_static()
{
  ads_list_t lst;
  ads_list_init(&lst, sizeof(int));

  ads_list_push_value(&lst, 10);
  ads_list_push_value(&lst, 8);
  ads_list_push_value(&lst, 6);
  ads_list_push_value(&lst, 4);
  ads_list_push_value(&lst, 2);
  ads_list_push_value(&lst, 0);

  test_assert(lst.size == 6);
  int i = 0;

  ads_list_for(&lst, it)
  {
    test_assert(ads_list_val(it, int) == i * 2);
    i++;
  }

  test_assert(i == 6);

  ads_list_clear(&lst);
  test_assert(lst.size == 0);
  test_assert(ads_list_empty(&lst));

  return TEST_RESULT_OK;
}

enum test_result_t list_for_each()
{
  ads_list_t lst;
  ads_list_init(&lst, sizeof(int));

  ads_list_push_value(&lst, 8);
  ads_list_push_value(&lst, 6);
  ads_list_push_value(&lst, -3);
  ads_list_push_value(&lst, 0);
  ads_list_push_value(&lst, 4);
  ads_list_push_value(&lst, 2);
  ads_list_push_value(&lst, 10);
  ads_list_push_value(&lst, -10);

  test_assert(lst.size == 8);

  int sum = 0;
  ads_list_for_each(&lst, it, sum += ads_list_val(it, int));

  test_assert(sum == 17);

  ads_list_clear(&lst);
  test_assert(lst.size == 0);
  test_assert(ads_list_empty(&lst));

  return TEST_RESULT_OK;
}

enum test_result_t list_erase()
{
  ads_list_auto lst = ads_list_create(int);

  ads_list_push_value(lst, 7);
  ads_list_push_value(lst, 6);
  ads_list_push_value(lst, 5);
  ads_list_push_value(lst, 4);
  ads_list_push_value(lst, 3);
  ads_list_push_value(lst, 2);
  ads_list_push_value(lst, 1);
  ads_list_push_value(lst, 0);

  test_assert(lst->size == 8);

  {
    ads_list_auto rmlst = ads_list_create(void*);

    ads_list_for(lst, it)
    {
      if (ads_list_val(it, int) % 2 == 0)
        ads_list_push_value(rmlst, it);
    }

    {
      void rmdeleter(void* ptr)
      {
        ads_list_erase(lst, *(void**) ptr);
      }
      ads_list_clear(rmlst, rmdeleter);
    }
  }

  test_assert(lst->size == 4);

  int i = 0;
  ads_list_for(lst, it)
  {
    int val = ads_list_val(it, int);

    switch (i)
    {
    case 0:
      test_assert(val == 1);
      break;
    case 1:
      test_assert(val == 3);
      break;
    case 2:
      test_assert(val == 5);
      break;
    case 3:
      test_assert(val == 7);
      break;
    default:
      test_assert(false);
    }

    i++;
  }

  return TEST_RESULT_OK;
}

enum test_result_t list_erase_if()
{
  ads_list_auto lst = ads_list_create(int);

  ads_list_push_value(lst, 7);
  ads_list_push_value(lst, 6);
  ads_list_push_value(lst, 5);
  ads_list_push_value(lst, 4);
  ads_list_push_value(lst, 3);
  ads_list_push_value(lst, 2);
  ads_list_push_value(lst, 1);
  ads_list_push_value(lst, 0);

  test_assert(lst->size == 8);

  {
    bool condition(int v)
    {
      return v % 2 == 0;
    }
    ads_list_erase_if(lst, int, condition);
  }
  test_assert(lst->size == 4);

  int i = 0;
  ads_list_for(lst, it)
  {
    int val = ads_list_val(it, int);

    switch (i)
    {
    case 0:
      test_assert(val == 1);
      break;
    case 1:
      test_assert(val == 3);
      break;
    case 2:
      test_assert(val == 5);
      break;
    case 3:
      test_assert(val == 7);
      break;
    default:
      test_assert(false);
    }

    i++;
  }

  return TEST_RESULT_OK;
}

static void list_contains_null_remover(void* ptr)
{
  if (*(void**) ptr)
    free(*(void**) ptr);
}

enum test_result_t list_contains_null()
{
  ads_list_t lst;
  ads_list_init(&lst, sizeof(void*));

  void* ptr = malloc(sizeof(int));
  ads_list_push(&lst, &ptr);
  ptr = malloc(sizeof(int));
  ads_list_push(&lst, &ptr);

  ads_list_clear(&lst, list_contains_null_remover);
  test_assert(lst.size == 0);
  test_assert(ads_list_empty(&lst));

  return TEST_RESULT_OK;
}

enum test_result_t list_nested()
{
  /**
   * creating nested list matrix-like (arrows from head of list):
   *
   *     8  ->  7  ->  6  ->|
   *   |
   *   V
   *     5  ->  4  ->  3  ->|
   *   |
   *   V
   *     2  ->  1  ->  0  ->|
   *   |
   *   -
   */

  ads_list_t* lst = ads_list_create(ads_list_t);

  for (int i = 0; i < 3; i++)
  {
    ads_list_t* nested = ads_list_push(lst, NULL);
    ads_list_init(nested, sizeof(int));
    ads_list_push_value(nested, i * 3 + 0);
    ads_list_push_value(nested, i * 3 + 1);
    ads_list_push_value(nested, i * 3 + 2);
  }

  test_assert(lst->size == 3);

  int sum = 0;
  int sum_2 = 0;

  ads_list_for(lst, m_it)
  {
    ads_list_t nested = ads_list_val(m_it, ads_list_t);
    ads_list_for_each(&nested, n_it, sum += ads_list_val(n_it, int));

    ads_list_for(&nested, n_it)
    {
      sum_2 += ads_list_val(n_it, int);
    }
  }

  test_assert(sum == 36);
  test_assert(sum_2 == 36);

  // first method to destroy nested lists
  /*
    while (!ads_list_empty(lst))
    {
      ads_list_clear(ads_list_top(lst));
      ads_list_pop(lst, NULL, 0);
    }
  */
  // but i used this method
  {
    void nested_list_clear_func(void* ptr)
    {
      ads_list_clear(ptr);
    }
    ads_list_clear(lst, nested_list_clear_func);
  }

  ads_list_destroy(&lst);
  test_assert(lst == NULL);

  return TEST_RESULT_OK;
}

// -------------------------------- dlist --------------------------------

enum test_result_t dlist_create()
{
  ads_dlist_t* lst = ads_dlist_create(int);
  test_assert(lst != NULL);
  test_assert(ads_dlist_empty(lst));
  test_assert(lst->size == 0);

  ads_dlist_destroy(&lst);
  test_assert(lst == NULL);

  return TEST_RESULT_OK;
}

enum test_result_t dlist_push_top_1()
{
  ads_dlist_t* lst = ads_dlist_create(int*);
  int* ptr = malloc(sizeof(int));
  *ptr = 228;

  test_assert(ads_dlist_empty(lst));
  test_assert(lst->size == 0);

  ads_dlist_push_back(lst, &ptr);
  test_assert(ads_dlist_top_front(lst) != NULL);
  test_assert(ads_dlist_top_back(lst) == ads_dlist_top_front(lst));
  test_assert(lst->size == 1);
  test_assert(!ads_dlist_empty(lst));

  int** top = ads_dlist_top_front(lst);
  test_assert(**top == 228);
  test_assert(lst->size == 1);

  int* back = NULL;
  ads_dlist_pop_back(lst, &back);
  test_assert(lst->size == 0);
  test_assert(ads_dlist_top_front(lst) == NULL);
  test_assert(ads_dlist_empty(lst));
  test_assert(back == ptr);

  free(back);
  ads_dlist_destroy(&lst);

  return TEST_RESULT_OK;
}

enum test_result_t dlist_push_top_2()
{
  ads_dlist_auto lst = ads_dlist_create(int);
  int ptr;

  test_assert(ads_dlist_empty(lst));
  test_assert((int*) ads_dlist_top_front(lst) == NULL);

  ads_dlist_push_back_value(lst, 4);
  test_assert(!ads_dlist_empty(lst));
  test_assert(*(int*) ads_dlist_top_back(lst) == 4);
  test_assert(*(int*) ads_dlist_top_front(lst) == 4);

  ads_dlist_push_back_value(lst, 8);
  test_assert(*(int*) ads_dlist_top_back(lst) == 8);
  test_assert(*(int*) ads_dlist_top_front(lst) == 4);

  ads_dlist_push_back_value(lst, 7);
  test_assert(*(int*) ads_dlist_top_back(lst) == 7);
  test_assert(*(int*) ads_dlist_top_front(lst) == 4);

  ads_dlist_pop_back(lst, &ptr);
  test_assert(!ads_dlist_empty(lst));
  test_assert(ptr == 7);
  test_assert(*(int*) ads_dlist_top_back(lst) == 8);
  test_assert(*(int*) ads_dlist_top_front(lst) == 4);

  ads_dlist_pop_back(lst, &ptr);
  test_assert(!ads_dlist_empty(lst));
  test_assert(ptr == 8);
  test_assert(*(int*) ads_dlist_top_back(lst) == 4);
  test_assert(*(int*) ads_dlist_top_front(lst) == 4);

  ads_dlist_pop_back(lst, &ptr);
  test_assert(ads_dlist_empty(lst));
  test_assert(ptr == 4);
  test_assert((int*) ads_dlist_top_back(lst) == NULL);

  ads_dlist_push_front_value(lst, 3);
  test_assert(!ads_dlist_empty(lst));
  test_assert(*(int*) ads_dlist_top_front(lst) == 3);
  test_assert(*(int*) ads_dlist_top_back(lst) == 3);

  ads_dlist_push_front_value(lst, 2);
  test_assert(!ads_dlist_empty(lst));
  test_assert(*(int*) ads_dlist_top_front(lst) == 2);
  test_assert(*(int*) ads_dlist_top_back(lst) == 3);

  ads_dlist_push_back_value(lst, 4);
  test_assert(!ads_dlist_empty(lst));
  test_assert(*(int*) ads_dlist_top_front(lst) == 2);
  test_assert(*(int*) ads_dlist_top_back(lst) == 4);

  int i = 0;
  ads_dlist_for_reverse(lst, it)
  {
    int val = ads_dlist_val(it, int);

    switch (i)
    {
    case 0:
      test_assert(val == 4);
      break;
    case 1:
      test_assert(val == 3);
      break;
    case 2:
      test_assert(val == 2);
      break;
    default:
      test_assert(false);
    }

    i++;
  }

  i = 0;
  ads_dlist_for(lst, it)
  {
    int val = ads_dlist_val(it, int);

    switch (i)
    {
    case 0:
      test_assert(val == 2);
      break;
    case 1:
      test_assert(val == 3);
      break;
    case 2:
      test_assert(val == 4);
      break;
    default:
      test_assert(false);
    }

    i++;
  }

  // ads_dlist_destroy(&lst);
  // test_assert(lst == NULL);

  return TEST_RESULT_OK;
}

enum test_result_t dlist_static()
{
  ads_dlist_t lst;
  ads_dlist_init(&lst, sizeof(int));

  ads_dlist_push_front_value(&lst, 4);
  ads_dlist_push_back_value(&lst, 6);
  ads_dlist_push_front_value(&lst, 2);
  ads_dlist_push_back_value(&lst, 8);
  ads_dlist_push_front_value(&lst, 0);
  ads_dlist_push_back_value(&lst, 10);

  test_assert(lst.size == 6);
  int i = 0;

  ads_dlist_for(&lst, it)
  {
    test_assert(ads_dlist_val(it, int) == i * 2);
    i++;
  }

  test_assert(i == 6);

  ads_dlist_clear(&lst);
  test_assert(lst.size == 0);
  test_assert(ads_dlist_empty(&lst));

  return TEST_RESULT_OK;
}

enum test_result_t dlist_erase()
{
  ads_dlist_auto lst = ads_dlist_create(int);

  for (int i = 0; i < 10; i++)
    ads_dlist_push_front_value(lst, i);

  test_assert(lst->size == 10);

  ads_dlist_erase_if(lst, int, ads_lambda(bool, (int v) {
                       return v % 3 == 0 || v % 3 == 1;
                     }));

  int i = 0;
  ads_dlist_for_reverse(lst, it)
  {
    int v = ads_dlist_val(it, int);

    switch (i)
    {
    case 0:
      test_assert(v == 2);
      break;
    case 1:
      test_assert(v == 5);
      break;
    case 2:
      test_assert(v == 8);
      break;
    default:
      test_assert(false);
      break;
    }

    i++;
  }

  test_assert(lst->size == 3);

  ads_dlist_erase(lst, lst->front);
  test_assert(lst->size == 2);
  i = 0;
  ads_dlist_for_reverse(lst, it)
  {
    int v = ads_dlist_val(it, int);

    switch (i)
    {
    case 0:
      test_assert(v == 2);
      break;
    case 1:
      test_assert(v == 5);
      break;
    default:
      test_assert(false);
      break;
    }

    i++;
  }

  ads_dlist_erase(lst, lst->back);
  test_assert(lst->size == 1);
  i = 0;
  ads_dlist_for_reverse(lst, it)
  {
    int v = ads_dlist_val(it, int);

    switch (i)
    {
    case 0:
      test_assert(v == 5);
      break;
    default:
      test_assert(false);
      break;
    }

    i++;
  }

  ads_dlist_erase(lst, lst->back);
  test_assert(lst->size == 0);
  i = 0;
  ads_dlist_for_reverse(lst, it)
  {
    i++;
  }
  test_assert(i == 0);

  return TEST_RESULT_OK;
}
