/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.
*/

#pragma once


namespace cm {


template<typename T>
class Range {
public:
    struct DefaultStep
    {};

    constexpr Range(T const& last)
        : Range(T{}, last)
    {}

    constexpr Range(T const& first, T const& last, T step = {})
        : _first(first),
          _last(last),
          _step(step == T{} ? 1 : step),
          _step_function(first > last ? ([](T step_, T& iterator) { iterator -= step_; }) : ([](T step_, T& iterator) {
              iterator += step_;
          }))
    {}


    struct Iterator
    {
        constexpr Iterator(Range const& range, T const& position)
            : _range(range), _position(position)
        {}

        constexpr Iterator& operator++()
        {
            _range._step_function(_range._step, this->_position);
            return *this;
        }

        constexpr bool operator==(Iterator const& other) const noexcept
        {
            return &_range == &other._range && _position == other._position;
        }

        constexpr T operator*() const noexcept { return _position; }

    private:
        Range const& _range;
        T _position;
    };

    constexpr Iterator begin() const { return Iterator(*this, _first); }
    constexpr Iterator end() const { return Iterator(*this, _last); }


private:
    T const _first;
    T const _last;
    T const _step;
    void (*_step_function)(T step, T& value);
};

template<typename T>
Range(T const&) -> Range<T>;
template<typename T>
Range(T const&, T const&) -> Range<T>;
// template<typename T, typename U>
// Range(T const&, U const&) -> Range<T>;

}  // namespace cm
