#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <utility>
#include <vector>
#include <functional>
using namespace std;

const string gap(1, '\t');

auto divide(int dividend, int divisor) {
    return make_pair(dividend / divisor, dividend % divisor);
}

int get_no_digits(int n) {
    return to_string(n).size();
}

using box_counts = pair<int, int>;  //no_incr_boxes, no_rem_boxes

box_counts get_no_boxes(int n, int incr) {
    const auto [quot, rem] = divide(n, incr);
    return {quot, rem? 1: 0};
}

box_counts max(box_counts a, box_counts b, box_counts c) {
    return {max(max(a.first, b.first), c.first), max(max(a.second, b.second), c.second)};
}

box_counts get_no_boxes(int a, int b, int incr) {
    const auto a_boxes = get_no_boxes(a, incr);
    const auto b_boxes = get_no_boxes(b, incr);
    const auto sum_boxes = get_no_boxes(a + b, 2 * incr);
    cout << a_boxes.second << b_boxes.second << sum_boxes.second << '\n';
    return max(a_boxes, b_boxes, sum_boxes);
}

auto generate_box_line(int n, char box_hz_char) {
    const int no_digits = get_no_digits(n);
    return string(no_digits + 2/*spaces*/ + 2/*box_vt_chars*/, box_hz_char);
}

auto generate_digit_line(int n, char box_vt_char) {
    return string(1, box_vt_char) + " " + to_string(n) + " " + box_vt_char;
}

auto generate_lines(int n, char box_hz_char, char box_vt_char) {
    return make_pair(generate_box_line(n, box_hz_char), 
        generate_digit_line(n, box_vt_char));
}

using repeat_string = pair<int, const string&>;

void draw_box(box_counts no_boxes, const repeat_string& incr_line, 
        const repeat_string& rem_line, auto out_it) {
    generate_n(out_it, no_boxes.first - incr_line.first, []{ return ""s; });
    generate_n(out_it, incr_line.first, [&]{ return incr_line.second; });
    if (no_boxes.second) {
        *out_it++ = rem_line.second;
    }
    *out_it++ = "\n";
}

void draw_digits(int n, box_counts no_boxes, const repeat_string& incr_line, 
        const repeat_string& rem_line, auto out_it) {
    generate_n(out_it, no_boxes.first - incr_line.first, []{ return ""s; });
    generate_n(out_it, incr_line.first, [&]{ return incr_line.second; });
    if (no_boxes.second) {
        *out_it++ = rem_line.second;
    }
    *out_it++ = " = ";
    *out_it++ = to_string(n);
    *out_it++ = "\n";
}

void show_n(const int n, const int incr, const box_counts no_boxes, 
        char box_hz_char, char box_vt_char) {
    const auto [no_incr, rem] = divide(n, incr);
    
    const auto [incr_box_line, incr_digits_line] = generate_lines(incr, box_hz_char, box_vt_char);
    const auto [rem_box_line, rem_digits_line] = generate_lines(rem, box_hz_char, box_vt_char);
    
    ostream_iterator<string> out(cout, gap.c_str());
    
    *out++ = "";    //start with an offset of gap
    draw_box(no_boxes, {no_incr, incr_box_line}, {rem? 1: 0, rem_box_line}, out);
    draw_digits(n, no_boxes, {no_incr, incr_digits_line}, {rem? 1: 0, rem_digits_line}, out);
    draw_box(no_boxes, {no_incr, incr_box_line}, {rem? 1: 0, rem_box_line}, out);
}

void show_add(int a, int b, char box_hz_char = '-', char box_vt_char = '|') {
    const int incr = 10;
    const int sum_incr = 2 * incr;
    
    const auto no_boxes = get_no_boxes(a, b, incr);
    cout << "no_boxes: " << no_boxes.first << ' ' << no_boxes.second << '\n';
    
    cout << "\033[1;32m";
    show_n(a, incr, no_boxes, box_hz_char, box_vt_char);
    cout << '\n';
    
    cout << "\033[1;36m";
    show_n(b, incr, no_boxes, box_hz_char, box_vt_char);
    cout << '\n';
    
    cout << "\033[1;31m";
    show_n(a + b, sum_incr, no_boxes, box_hz_char, box_vt_char);
    cout << "\n\033[0m";
}

//using fun_2_num = function<void(int, int, char, char)>;
using fun_2_num = void (*)(int, int, char, char);
vector<pair<fun_2_num, string>> fun_names = {
    {show_add, "add"}
};

const string& lookup_name(const fun_2_num f) {
    auto it = find_if(begin(fun_names), end(fun_names), [&f](const auto& entry) {
        return entry.first == f;
    });
    static const string unknown = "UNKNOWN";
    return it != end(fun_names)? it->second: unknown;
}

template<typename F>
void loop_2_nums_activity(F&& activity) {
    while (1) {
        cout << "Enter the numbers to " << lookup_name(activity) << ": ";
        int a, b;
        cin >> a >> b;
        if (cin.fail()) {
            break;
        }
        activity(a, b, '-', '|');
    }
}

int main()
{
    //while (1) {
        //prompt user for activity
        loop_2_nums_activity(show_add);
    //}
    cout << "\033[1;33mBYE!\n\033[0m";
}
