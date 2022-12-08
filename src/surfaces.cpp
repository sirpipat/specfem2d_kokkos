#include "../include/surfaces.h"
#include "../include/fortran_IO.h"
#include "../include/specfem_mpi.h"

specfem::surfaces::acoustic_free_surface::acoustic_free_surface(
    const int nelem_acoustic_surface) {
  if (nelem_acoustic_surface > 0) {
    this->numacfree_surface = specfem::HostView1d<int>(
        "specfem::mesh::acoustic_free_surface::numacfree_surface",
        nelem_acoustic_surface);
    this->typeacfree_surface = specfem::HostView1d<int>(
        "specfem::mesh::acoustic_free_surface::typeacfree_surface",
        nelem_acoustic_surface);
    this->e1 = specfem::HostView1d<int>(
        "specfem::mesh::acoustic_free_surface::e1", nelem_acoustic_surface);
    this->e2 = specfem::HostView1d<int>(
        "specfem::mesh::acoustic_free_surface::e2", nelem_acoustic_surface);
    this->ixmin = specfem::HostView1d<int>(
        "specfem::mesh::acoustic_free_surface::ixmin", nelem_acoustic_surface);
    this->ixmax = specfem::HostView1d<int>(
        "specfem::mesh::acoustic_free_surface::ixmax", nelem_acoustic_surface);
    this->izmin = specfem::HostView1d<int>(
        "specfem::mesh::acoustic_free_surface::izmin", nelem_acoustic_surface);
    this->izmax = specfem::HostView1d<int>(
        "specfem::mesh::acoustic_free_surface::izmax", nelem_acoustic_surface);
  }
  return;
}

specfem::surfaces::acoustic_free_surface::acoustic_free_surface(
    std::ifstream &stream, const int nelem_acoustic_surface,
    const specfem::MPI::MPI *mpi) {

  std::vector<int> acfree_edge(4, 0);
  *this = specfem::surfaces::acoustic_free_surface(nelem_acoustic_surface);

  if (nelem_acoustic_surface > 0) {
    for (int inum = 0; inum < nelem_acoustic_surface; inum++) {
      IO::fortran_IO::fortran_read_line(stream, &acfree_edge);
      this->numacfree_surface(inum) = acfree_edge[0];
      this->typeacfree_surface(inum) = acfree_edge[1];
      this->e1(inum) = acfree_edge[2];
      this->e2(inum) = acfree_edge[3];
    }
  }

  mpi->sync_all();
  return;
}