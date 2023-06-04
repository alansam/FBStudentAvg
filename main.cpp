/*
Write a program that reads the student information from a tab separated values (tsv) file. The program then creates a text file that records the course grades of the students. Each row of the tsv file contains the Last Name, First Name, Midterm1 score, Midterm2 score, and the Final score of a student. A sample of the student information is provided in StudentInfo.tsv. Assume the number of students is at least 1 and at most 20. Assume also the last names and first names do not contain whitespaces.

The program performs the following tasks:

Read the file name of the tsv file from the user.
Open the tsv file and read the student information.
Compute the average exam score of each student.
Assign a letter grade to each student based on the average exam score in the following scale:
A: 90 =< x
B: 80 =< x < 90
C: 70 =< x < 80
D: 60 =< x < 70
F: x < 60
Compute the average of each exam.
Output the last names, first names, exam scores, and letter grades of the students into a text file named report.txt. Output one student per row and separate the values with a tab character.
Output the average of each exam, with two digits after the decimal point, at the end of report.txt. Hint: Use the setprecision manipulator to format the output.
Ex: If the input of the program is:

StudentInfo.tsv
and the contents of StudentInfo.tsv are:

Barrett    Edan    70  45  59
Bradshaw    Reagan  96  97  88
Charlton    Caius   73  94  80
Mayo    Tyrese  88  61  36
Stern    Brenda  90  86  45
the file report.txt should contain:

Barrett    Edan    70  45  59  F
Bradshaw    Reagan  96  97  88  A
Charlton    Caius   73  94  80  B
Mayo    Tyrese  88  61  36  D
Stern    Brenda  90  86  45  C

Averages: midterm1 83.40, midterm2 76.60, final 61.60

I’ve been working on this for a little while now, and I can get it to write to the file with the proper letter grade but I’m unable to get it to write the avg of all the grades like above. Thank you!!!
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <algorithm>
#include <numeric>

using namespace std::literals::string_literals;

enum scores : size_t { m1, m2, finl, };
struct student_info {
  std::string name_last;
  std::string name_first;
  uint32_t score[scores::finl + 1];
  char grade;
  double average;

  student_info() {
    std::clog << "student_info::" << __func__ << "() default c'tor\n"s;
  }
  ~student_info() {
    std::clog << "student_info::" << __func__ << "() d'tor\n"s;
  }
};

std::string student_info_fn_dflt = "studentinfo.tsv"s;
std::string reportfn_dflt = "report.txt"s;

/*
 *  MARK:  get_data()
 */
void get_data(std::vector<std::unique_ptr<student_info>> & siv, std::string const & sif_nm) {
  std::ifstream f_si(sif_nm);
  if (f_si.is_open()) {
    while (!f_si.eof()) {
      auto sip = std::make_unique<student_info>();
      f_si >> sip->name_last
           >> sip->name_first
           >> sip->score[scores::m1]
           >> sip->score[scores::m2]
           >> sip->score[scores::finl];
      siv.push_back(std::move(sip));
    }

    f_si.close();
  }

  return;
}

/*
 *  MARK:  report()
 */
void report(std::vector<std::unique_ptr<student_info>> const & siv, std::string const & rpt_nm) {
  std::ofstream rpt(rpt_nm);
  if (rpt.is_open()) {
    for (auto const & spi : siv) {
      rpt << std::fixed << std::setprecision(2);
      rpt << std::setw(16) << spi->name_last
          << std::setw(16) << spi->name_first
          << std::setw(3) << spi->score[scores::m1]
          << std::setw(3) << spi->score[scores::m2]
          << std::setw(3) << spi->score[scores::finl]
          << std::setw(2) << spi->grade
          << std::setw(6) << spi->average
          << '\n';
    }

    rpt.close();
  }
  return;
}
/*
 *  MARK:  main()
 */
int main() {
  std::cout << "FB.StudentAvg\n"s;

  std::vector<std::unique_ptr<student_info>> siv;
  get_data(siv, student_info_fn_dflt);
  for (auto const & spi : siv) {
    std::cout << spi->name_last
              << ' '
              << spi->name_first
              << ' '
              << spi->score[scores::m1]
              << ' '
              << spi->score[scores::m2]
              << ' '
              << spi->score[scores::finl]
              << '\n';
  }

  std::for_each(siv.begin(), siv.end(), [](auto & spi) {
    auto sc = std::vector<double> {
      1.0 * spi->score[scores::m1],
      1.0 * spi->score[scores::m2],
      1.0 * spi->score[scores::finl], };
    auto avg = std::reduce(sc.begin(), sc.end()) / sc.size();
    std::cout << std::fixed << std::setprecision(3);
    auto grady = [](auto avg) {
      /*
        A: 90 =< x
        B: 80 =< x < 90
        C: 70 =< x < 80
        D: 60 =< x < 70
        F: x < 60
       */
      auto grade = (avg >= 90.0) ? 'A'
                 : (avg >= 80.0) ? 'B'
                 : (avg >= 70.0) ? 'C'
                 : (avg >= 60.0) ? 'D'
                 : 'F';
      return grade;
    };
    spi->grade = grady(avg);
    spi->average = avg;
    std::cout << std::setw(16) << spi->name_last
              << std::setw(16) << spi->name_first
              << std::setw(3) << spi->score[scores::m1]
              << std::setw(3) << spi->score[scores::m2]
              << std::setw(3) << spi->score[scores::finl]
              << std::setw(2) << spi->grade
              << std::setw(8) << spi->average
              << '\n';
  });

  report(siv, reportfn_dflt);

  return 0;
}
