
#include "../include/compare_array.h"
#include "../../../../include/fortran_IO.h"
#include "../../../../include/kokkos_abstractions.h"
#include <Kokkos_Core.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void specfem::testing::equate(int computed_value, int ref_value) {
  if (computed_value != ref_value) {
    std::ostringstream ss;
    ss << "Computed value " << computed_value << " != ref value" << ref_value;

    throw std::runtime_error(ss.str());
  }
};

void specfem::testing::equate(type_real computed_value, type_real ref_value,
                              type_real tol) {

  if (fabs(computed_value - ref_value) >= tol) {
    std::ostringstream ss;
    ss << "Computed value = " << computed_value
       << " != ref value = " << ref_value << " with a tolerance of " << tol;

    throw std::runtime_error(ss.str());
  }
};

void specfem::testing::test_array(specfem::HostView1d<int> computed_array,
                                  std::string ref_file, int n1) {
  assert(computed_array.extent(0) == n1);

  int ref_value;
  std::ifstream stream;
  stream.open(ref_file);

  for (int i1 = 0; i1 < n1; i1++) {
    IO::fortran_IO::fortran_read_line(stream, &ref_value);
    try {
      equate(computed_array(i1), ref_value);
    } catch (std::runtime_error &e) {
      stream.close();
      std::ostringstream ss;
      ss << e.what() << ", at i1 = " << i1;
      throw std::runtime_error(ss.str());
    }
  }
}

void specfem::testing::test_array(specfem::HostView2d<int> computed_array,
                                  std::string ref_file, int n1, int n2) {
  assert(computed_array.extent(0) == n1);
  assert(computed_array.extent(1) == n2);

  int ref_value;
  std::ifstream stream;
  stream.open(ref_file);

  for (int i1 = 0; i1 < n1; i1++) {
    for (int i2 = 0; i2 < n2; i2++) {
      IO::fortran_IO::fortran_read_line(stream, &ref_value);
      try {
        equate(computed_array(i1, i2), ref_value);
      } catch (std::runtime_error &e) {
        stream.close();
        std::ostringstream ss;
        ss << e.what() << ", at n1 = " << i1 << ", n2 = " << i2;
        throw std::runtime_error(ss.str());
      }
    }
  }
}

void specfem::testing::test_array(specfem::HostView3d<int> computed_array,
                                  std::string ref_file, int n1, int n2,
                                  int n3) {
  assert(computed_array.extent(0) == n1);
  assert(computed_array.extent(1) == n2);
  assert(computed_array.extent(2) == n3);

  int ref_value;
  std::ifstream stream;
  stream.open(ref_file);

  for (int i1 = 0; i1 < n1; i1++) {
    for (int i2 = 0; i2 < n2; i2++) {
      for (int i3 = 0; i3 < n3; i3++) {
        IO::fortran_IO::fortran_read_line(stream, &ref_value);
        try {
          equate(computed_array(i1, i2, i3), ref_value);
        } catch (std::runtime_error &e) {
          stream.close();
          std::ostringstream ss;
          ss << e.what() << ", at n1 = " << i1 << ", n2 = " << i2
             << ", n3 = " << i3;
          throw std::runtime_error(ss.str());
        }
      }
    }
  }
}

void specfem::testing::test_array(specfem::HostView1d<type_real> computed_array,
                                  std::string ref_file, int n1) {
  assert(computed_array.extent(0) == n1);

  type_real max_val = std::numeric_limits<type_real>::min();
  type_real min_val = std::numeric_limits<type_real>::max();

  for (int i1 = 0; i1 < n1; i1++) {
    if (max_val < computed_array(i1))
      max_val = computed_array(i1);
    if (min_val > computed_array(i1))
      min_val = computed_array(i1);
  }

  type_real tol = 1e-2 * fabs(max_val + min_val) / 2;

  type_real ref_value;
  std::ifstream stream;
  stream.open(ref_file);

  for (int i1 = 0; i1 < n1; i1++) {
    IO::fortran_IO::fortran_read_line(stream, &ref_value);
    try {
      equate(computed_array(i1), ref_value, tol);
    } catch (std::runtime_error &e) {
      stream.close();
      std::ostringstream ss;
      ss << e.what() << ", at i1 = " << i1;
      throw std::runtime_error(ss.str());
    }
  }
}

void specfem::testing::test_array(specfem::HostView2d<type_real> computed_array,
                                  std::string ref_file, int n1, int n2) {
  assert(computed_array.extent(0) == n1);
  assert(computed_array.extent(1) == n2);

  type_real max_val = std::numeric_limits<type_real>::min();
  type_real min_val = std::numeric_limits<type_real>::max();

  for (int i1 = 0; i1 < n1; i1++) {
    for (int i2 = 0; i2 < n2; i2++) {
      if (max_val < computed_array(i1, i2))
        max_val = computed_array(i1, i2);
      if (min_val > computed_array(i1, i2))
        min_val = computed_array(i1, i2);
    }
  }

  type_real tol = 1e-2 * fabs(max_val + min_val) / 2;

  type_real ref_value;
  std::ifstream stream;
  stream.open(ref_file);

  for (int i1 = 0; i1 < n1; i1++) {
    for (int i2 = 0; i2 < n2; i2++) {
      IO::fortran_IO::fortran_read_line(stream, &ref_value);
      try {
        equate(computed_array(i1, i2), ref_value, tol);
      } catch (std::runtime_error &e) {
        stream.close();
        std::ostringstream ss;
        ss << e.what() << ", at n1 = " << i1 << ", n2 = " << i2;
        throw std::runtime_error(ss.str());
      }
    }
  }
}

void specfem::testing::test_array(specfem::HostView3d<type_real> computed_array,
                                  std::string ref_file, int n1, int n2,
                                  int n3) {
  assert(computed_array.extent(0) == n1);
  assert(computed_array.extent(1) == n2);
  assert(computed_array.extent(2) == n3);

  type_real max_val = std::numeric_limits<type_real>::min();
  type_real min_val = std::numeric_limits<type_real>::max();

  for (int i1 = 0; i1 < n1; i1++) {
    for (int i2 = 0; i2 < n2; i2++) {
      for (int i3 = 0; i3 < n3; i3++) {
        if (max_val < computed_array(i1, i2, i3))
          max_val = computed_array(i1, i2, i3);
        if (min_val > computed_array(i1, i2, i3))
          min_val = computed_array(i1, i2, i3);
      }
    }
  }

  type_real tol = 1e-2 * fabs(max_val + min_val) / 2;

  type_real ref_value;
  std::ifstream stream;
  stream.open(ref_file);

  for (int i1 = 0; i1 < n1; i1++) {
    for (int i2 = 0; i2 < n2; i2++) {
      for (int i3 = 0; i3 < n3; i3++) {
        IO::fortran_IO::fortran_read_line(stream, &ref_value);
        try {
          equate(computed_array(i1, i2, i3), ref_value, tol);
        } catch (std::runtime_error &e) {
          stream.close();
          std::ostringstream ss;
          ss << e.what() << ", at n1 = " << i1 << ", n2 = " << i2
             << ", n3 = " << i3;
          throw std::runtime_error(ss.str());
        }
      }
    }
  }
}
