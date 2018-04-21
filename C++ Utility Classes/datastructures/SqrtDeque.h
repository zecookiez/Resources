#ifndef DATASTRUCTURES_SQRTDEQUE_H_
#define DATASTRUCTURES_SQRTDEQUE_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Sqrt Array:
 * Decomposes the array into deques of a specificed block size (around 2 to 10 times
 * sqrt(n) is optimal).
 *
 * Usage:
 * SqrtDeque<4096, int> arr;
 *
 * Initializing: O(N)
 * Insert: O(sqrt(N))
 * Erase: O(sqrt(N))
 * Push Front, Pop Front: O(1) amortized
 * Push Back, Pop Back: O(1) amortized
 * At, Accessor, Mutator: O(1)
 * Front, Back: O(1)
 * Lower Bound, Upper Bound, Floor, Ceiling, Above, Below: O(log(N))
 * Empty, Size: O(1)
 * Values: O(N)
 * Sort: O(N log N) time, O(N) extra space
 */
template <const int BLOCK_SIZE, typename Value, typename SmallAlloc = allocator<Value>,
        typename LargeAlloc = allocator<vector<Value>>, typename IntAlloc = allocator<int>>
struct SqrtDeque {
private:
    int n;
    deque<deque<Value, SmallAlloc>, LargeAlloc> a;

public:
    /**
     * Initializes an empty structure.
     */
    SqrtDeque() : n(0) {}

    /**
     * Initializes the structure with an initial size.
     *
     * @param n the initial size
     */
    SqrtDeque(const int n) : n(n) {
        assert(n >= 0);
        for (int i = 0; i < n; i += BLOCK_SIZE) {
            a.emplace_back(min(BLOCK_SIZE, n - i));
        }
    }

    /**
     * Initializes the structures with the elements between st and en
     * such that st <= en.
     *
     * @param st the starting iterator (inclusive)
     * @param en the ending iterator (exclusive)
     */
    template <typename It>
    SqrtDeque(const It st, const It en) : n(en - st) {
        assert(n >= 0);
        for (It i = st; i < en; i += BLOCK_SIZE) {
            a.emplace_back(i, min(i + BLOCK_SIZE, en));
        }
    }

    /**
     * Initializes the structures with an initializer list. The elements must be sorted.
     *
     * @param il the initializer list
     */
    SqrtDeque(initializer_list<Value> il) : n(il.end() - il.begin()) {
        assert(n >= 0);
        for (auto i = il.begin(); i < il.end(); i += BLOCK_SIZE) {
            a.emplace_back(i, min(i + BLOCK_SIZE, il.end()));
        }
    }

    /**
     * Inserts a value before the current kth index of the structure. If k == n,
     * then the value is inserted at the end of the structure.
     *
     * @param k the 0-based index to insert before, 0 <= k <= n
     * @param val the value to be inserted
     */
    void insert(int k, const Value val) {
        assert(0 <= k && k <= n);
        if (n == 0) a.emplace_back();
        int i;
        if (k == n) i = (int) a.size();
        else if (k < (int) a[0].size()) i = 0;
        else {
            k -= (int) a[0].size();
            i = 1 + k / BLOCK_SIZE;
            k %= BLOCK_SIZE;
        }
        n++;
        if (i == (int) a.size()) a[--i].push_back(val);
        else a[i].insert(a[i].begin() + k, val);
        if (i < (int) a.size() / 2) {
            for (int j = i - 1; j >= 0; j--) {
                a[j].push_back(a[j + 1].front());
                a[j + 1].pop_front();
            }
            if ((int) a.front().size() > BLOCK_SIZE) {
                a.emplace_front();
                a.front().push_back(a[1].front());
                a[1].pop_front();
            }
        } else {
            for (int j = i + 1; j < (int) a.size(); j++) {
                a[j].push_front(a[j - 1].back());
                a[j - 1].pop_back();
            }
            if ((int) a.back().size() > BLOCK_SIZE) {
                a.emplace_back();
                a.back().push_front(a[((int) a.size()) - 2].back());
                a[((int) a.size()) - 2].pop_back();
            }
        }
    }

    /**
     * Erases the kth value in the structure.
     *
     * @param k the value to erase, 0 <= k < n
     */
    void erase(int k) {
        assert(0 <= k && k < n);
        --n;
        int i;
        if (k < (int) a[0].size()) i = 0;
        else {
            k -= (int) a[0].size();
            i = 1 + k / BLOCK_SIZE;
            k %= BLOCK_SIZE;
        }
        a[i].erase(a[i].begin() + k);
        if (i < (int) a.size() / 2) {
            for (int j = i - 1; j >= 0; j--) {
                a[j + 1].push_front(a[j].back());
                a[j].pop_back();
            }
            if (a.front().empty()) a.pop_front();
        } else {
            for (int j = i + 1; j < (int) a.size(); j++) {
                a[j - 1].push_back(a[j].front());
                a[j].pop_front();
            }
            if (a.back().empty()) a.pop_back();
        }
    }

    /**
     * Returns a constant reference to the first element.
     * @return a constant reference to the first element
     */
    const Value &front() const {
        assert(n > 0);
        return a.front().front();
    }

    /**
     * Erases the first element in the structure.
     */
    void pop_front() {
        assert(n > 0);
        --n;
        a.front().pop_front();
        if (a.front().empty()) a.pop_front();
    }

    /**
     * Inserts an element at the front of the structure.
     * @param val the value to be inserted
     */
    void push_front(const Value &val) {
        if (n++ == 0) a.emplace_front();
        a.front().push_front(val);
        if ((int) a.front().size() > BLOCK_SIZE) {
            a.emplace_front();
            a.front().push_back(a[1].front());
            a[1].pop_front();
        }
    }

    /**
     * Returns a constant reference to the last element.
     * @return a constant reference to the last element
     */
    const Value &back() const {
        assert(n > 0);
        return a.back().back();
    }

    /**
     * Erases the last element in the structure.
     */
    void pop_back() {
        assert(n > 0);
        --n;
        a.back().pop_back();
        if (a.back().empty()) a.pop_back();
    }

    /**
     * Inserts an element at the back of the structure.
     * @param val the value to be inserted
     */
    void push_back(const Value &val) {
        if (n++ == 0) a.emplace_back();
        a.back().push_back(val);
        if ((int) a.back().size() > BLOCK_SIZE) {
            a.emplace_back();
            a.back().push_front(a[((int) a.size()) - 2].back());
            a[((int) a.size()) - 2].pop_back();
        }
    }

    /**
     * Mutator operator.
     *
     * @param k the 0-based index
     * @return a reference to the kth element in the structure
     */
    Value &operator [](int k) {
        assert(0 <= k && k < n);
        if (k < (int) a[0].size()) return a[0][k];
        int m = k - (int) a[0].size();
        return a[1 + m / BLOCK_SIZE][m % BLOCK_SIZE];
    }

    /**
     * Returns a constant reference to the kth value in the structure.
     *
     * @param k the 0-based index
     * @return a constant reference to the kth value in the structure
     */
    const Value &at(const int k) const {
        assert(0 <= k && k < n);
        if (k < (int) a[0].size()) return a[0][k];
        int m = k - (int) a[0].size();
        return a[1 + m / BLOCK_SIZE][m % BLOCK_SIZE];
    }

    /**
     * Accessor operator.
     * Returns a constant reference to the kth value in the structure.
     *
     * @param k the 0-based index
     * @return a constant reference to the kth value in the structure
     */
    const Value &operator [](const int k) const {
        return at(k);
    }

    /**
     * Checks if the structure is empty.
     *
     * @return true if the structure is empty, false otherwise
     */
    bool empty() const {
        return n == 0;
    }

    /**
     * Returns the number of values in the structure.
     *
     * @return the number of values in the structure
     */
    int size() const {
        return n;
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * less than or equal to val by the < operator. Identical to ceiling.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a pair containing the index and value of the smallest value
     * greater than or equal to val
     * @throws no_such_element_exception if val is larger than the largest value
     * in the structure
     */
    pair<int, Value> lower_bound(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (a[mid].back() < val) lo = mid + 1;
            else hi = mid;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to lower_bound() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (a[i][mid] < val) lo = mid + 1;
            else hi = mid;
        }
        if (i == 0) return {lo, a[i][lo]};
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * less than or equal to val based on the comparator. Identical to ceiling.
     *
     * @pre the structure must be sorted based on the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a pair containing the index and value of the smallest value
     * greater than or equal to val
     * @throws no_such_element_exception if val is larger than the largest value
     * in the structure
     */
    template <typename Comparator> pair<int, Value> lower_bound(const Value val, Comparator cmp) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].back(), val)) lo = mid + 1;
            else hi = mid;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to lower_bound() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[i][mid], val)) lo = mid + 1;
            else hi = mid;
        }
        if (i == 0) return {lo, a[i][lo]};
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * greater than to val by the < operator. Identical to above.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a pair containing the index and value of the smallest value
     * less than or equal to val
     * @throws no_such_element_exception if val is greater than or equal to
     * the largest value in the structure
     */
    pair<int, Value> upper_bound(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[mid].back()) hi = mid;
            else lo = mid + 1;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to upper_bound() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[i][mid]) hi = mid;
            else lo = mid + 1;
        }
        if (i == 0) return {lo, a[i][lo]};
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * greater than to val based on the comparator. Identical to above.
     *
     * @pre the structure must be sorted based on the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a pair containing the index and value of the smallest value
     * less than or equal to val
     * @throws no_such_element_exception if val is greater than or equal to
     * the largest value in the structure
     */
    template <typename Comparator> pair<int, Value> upper_bound(const Value val, Comparator cmp) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].back())) hi = mid;
            else lo = mid + 1;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to upper_bound() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) hi = mid;
            else lo = mid + 1;
        }
        if (i == 0) return {lo, a[i][lo]};
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the largest value
     * less than or equal to val by the < operator.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a pair containing the index and value of the largest value
     * less than or equal to val
     * @throws no_such_element_exception if val is less than the smallest value
     * in the structure
     */
    pair<int, Value> floor(const Value val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[mid].front()) hi = mid - 1;
            else lo = mid + 1;
        }
        if (hi == -1) throw no_such_element_exception("call to floor() resulted in no such value");
        int i = hi;
        lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[i][mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        if (i == 0) return {hi, a[i][hi]};
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + hi, a[i][hi]};
    }

    /**
     * Returns a pair containing the index and value of the largest value
     * less than or equal to val based on the comparator.
     *
     * @pre the structure must be sorted based on the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a pair containing the index and value of the largest value
     * less than or equal to val
     * @throws no_such_element_exception if val is less than the smallest value
     * in the structure
     */
    template <typename Comparator> pair<int, Value> floor(const Value val, Comparator cmp) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].front())) hi = mid - 1;
            else lo = mid + 1;
        }
        if (hi == -1) throw no_such_element_exception("call to floor() resulted in no such value");
        int i = hi;
        lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) hi = mid - 1;
            else lo = mid + 1;
        }
        if (i == 0) return {hi, a[i][hi]};
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + hi, a[i][hi]};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * less than or equal to val by the < operator. Identical to lower_bound.
     *
     * @pre the structure must be sorted
     * @param val the value
     * @return a pair containing the index and value of the smallest value
     * greater than or equal to val
     * @throws no_such_element_exception if val is greater than the largest value
     * in the structure
     */
    pair<int, Value> ceiling(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (a[mid].back() < val) lo = mid + 1;
            else hi = mid;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to ceiling() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (a[i][mid] < val) lo = mid + 1;
            else hi = mid;
        }
        if (i == 0) return {lo, a[i][lo]};
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * less than or equal to val based on the comparator. Identical to lower_bound.
     *
     * @pre the structure must be sorted based on the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a pair containing the index and value of the smallest value
     * greater than or equal to val
     * @throws no_such_element_exception if val is greater than the largest value
     * in the structure
     */
    template <typename Comparator> pair<int, Value> ceiling(const Value val, Comparator cmp) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].back(), val)) lo = mid + 1;
            else hi = mid;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to ceiling() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[i][mid], val)) lo = mid + 1;
            else hi = mid;
        }
        if (i == 0) return {lo, a[i][lo]};
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * greater than to val by the < operator. Identical to upper_bound.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a pair containing the index and value of the smallest value
     * less than or equal to val
     * @throws no_such_element_exception if val is greater than or equal to
     * the largest value in the structure
     */
    pair<int, Value> above(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[mid].back()) hi = mid;
            else lo = mid + 1;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to above() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[i][mid]) hi = mid;
            else lo = mid + 1;
        }
        if (i == 0) return {lo, a[i][lo]};
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the smallest value
     * greater than to val based on the comparator. Identical to upper_bound.
     *
     * @pre the structure must be sorted based on the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a pair containing the index and value of the smallest value
     * less than or equal to val
     * @throws no_such_element_exception if val is greater than or equal to
     * the largest value in the structure
     */
    template <typename Comparator> pair<int, Value> above(const Value val, Comparator cmp) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].back())) hi = mid;
            else lo = mid + 1;
        }
        if (lo == (int) a.size()) throw no_such_element_exception("call to above() resulted in no such value");
        int i = lo;
        lo = 0, hi = (int) a[i].size();
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) hi = mid;
            else lo = mid + 1;
        }
        if (i == 0) return {lo, a[i][lo]};
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + lo, a[i][lo]};
    }

    /**
     * Returns a pair containing the index and value of the largest value
     * less than val by the < operator.
     *
     * @pre the structure must be sorted by the < operator
     * @param val the value
     * @return a pair containing the index and value of the largest value
     * less than val
     * @throws no_such_element_exception if val is less than or equal to the smallest value
     * in the structure
     */
    pair<int, Value> below(const Value val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (a[mid].front() < val) lo = mid + 1;
            else hi = mid - 1;
        }
        if (hi == -1) throw no_such_element_exception("call to below() resulted in no such value");
        int i = hi;
        lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (val < a[i][mid]) lo = mid + 1;
            else hi = mid - 1;
        }
        if (i == 0) return {hi, a[i][hi]};
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + hi, a[i][hi]};
    }

    /**
     * Returns a pair containing the index and value of the largest value
     * less than val based on the comparator.
     *
     * @pre the structure must be sorted based on the comparator
     * @param val the value
     * @param cmp the comparator
     * @return a pair containing the index and value of the largest value
     * less than val
     * @throws no_such_element_exception if val is less than or equal to the smallest value
     * in the structure
     */
    template <typename Comparator> pair<int, Value> below(const Value val, Comparator cmp) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].front(), val)) lo = mid + 1;
            else hi = mid - 1;
        }
        if (hi == -1) throw no_such_element_exception("call to below() resulted in no such value");
        int i = hi;
        lo = 0, hi = ((int) a[i].size()) - 1;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[i][mid])) lo = mid + 1;
            else hi = mid - 1;
        }
        if (i == 0) return {hi, a[i][hi]};
        return {(int) a[0].size() + (i - 1) * BLOCK_SIZE + hi, a[i][hi]};
    }

    /**
     * Returns all values in the structure.
     *
     * @return a vector containing all values in the structure
     */
    vector<Value, SmallAlloc> values() const {
        vector<Value, SmallAlloc> ret;
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) a[i].size(); j++) {
                ret.push_back(a[i][j]);
            }
        }
        return ret;
    }

    /**
     * Sorts the structure by creating an auxiliary array.
     */
    void sort() {
        vector<Value, SmallAlloc> b;
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) a[i].size(); j++) {
                b.push_back(a[i][j]);
            }
        }
        std::sort(b.begin(), b.end());
        int k = 0;
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) a[i].size(); j++) {
                a[i][j] = b[k++];
            }
        }
    }

    /**
     * Sorts the structure with a comparator by creating an auxiliary array.
     * @param cmp the comparator
     */
    template <typename Comparator> void sort(Comparator cmp) {
        vector<Value, SmallAlloc> b;
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) a[i].size(); j++) {
                b.push_back(a[i][j]);
            }
        }
        std::sort(b.begin(), b.end(), cmp);
        int k = 0;
        for (int i = 0; i < (int) a.size(); i++) {
            for (int j = 0; j < (int) a[i].size(); j++) {
                a[i][j] = b[k++];
            }
        }
    }
};

#endif /* DATASTRUCTURES_SQRTDEQUE_H_ */
