#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <utility>
#include <vector>
#include <functional>
using namespace std;

auto divide(int dividend, int divisor) {
    return make_pair(dividend / divisor, dividend % divisor);
}

int get_no_digits(int n) {
    return to_string(n).size();
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

void draw_box(int count, const string& incr_line, const string& rem_line, auto out_it) {
    generate_n(out_it, count, [&]{ return incr_line; });
    *out_it++ = rem_line;
    *out_it++ = "\n";
}

void draw_digits(int n, int count, const string& incr_line, const string& rem_line, auto out_it) {
    generate_n(out_it, count, [&]{ return incr_line; });
    *out_it++ = rem_line;
    *out_it++ = " = ";
    *out_it++ = to_string(n);
    *out_it++ = "\n";
}

void show_n(const int n, const int incr, const int no_boxes, char box_hz_char, char box_vt_char) {
    const auto [no_incr, rem] = divide(n, incr);
    
    const auto [incr_box_line, incr_digits_line] = generate_lines(incr, box_hz_char, box_vt_char);
    const auto [rem_box_line, rem_digits_line] = generate_lines(rem, box_hz_char, box_vt_char);
    
    const string gap(1, '\t');
    
    ostream_iterator<string> out(cout, gap.c_str());
    
    *out++ = "";    //start with an offset of gap
    draw_box(no_incr, incr_box_line, rem_box_line, out);
    draw_digits(n, no_incr, incr_digits_line, rem_digits_line, out);
    draw_box(no_incr, incr_box_line, rem_box_line, out);
}

void show_add(int a, int b, char box_hz_char = '-', char box_vt_char = '|') {
    const int incr = 10;
    const int sum_incr = 2 * incr;
    const int no_boxes = max(max(get_no_digits(a), get_no_digits(b)),
        get_no_digits(a+b));
    
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
        cout << "Enter the numbers to " << lookup_name(activity)/*activity_name*/ << ": ";
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
