//---------------------------------------------------------------------------//
/*!
 * \file Mesh.hh
 */
//---------------------------------------------------------------------------//

#ifndef FEA_MESH_HH
#define FEA_MESH_HH

#include "Element.hh"

#include <array>
#include <vector>

namespace FEA
{

//---------------------------------------------------------------------------//
/*!
 * \class Mesh
 *
 * \brief Uniform mesh.
 */
class Mesh
{
  public:

    // Constructor.
    Mesh( const int n );

    // Get the total number of elements in the mesh.
    int totalNumElements() const;

    // Get the total number of nodes in the mesh.
    int totalNumNodes() const;

    // Get the width of each element in the mesh. 
    double getElementWidth() const;

    // Given a node id get its coordinates.
    void nodeCoordinate( const int node_id,
                          double& coord ) const;

    // Given an element id get its left and right endpoints. 
    void elementCoordinates( const int element_id,
                            std::array<double,2>& coords ) const;

    // Given a cardinal element id intitalize the element.
    void initializeElement( const int element_id,
                                  Element& element ) const;

    // Given a point in the domain, locate the cardinal index of the element
    // in which it is located
    void locateX( const double& x,
                  int& element_id ) const;

    // Map the value x from the global frame to the local frame of the element
    // in which it is located.
    void mapGlobalToLocalFrame(
        const double& x,
        const int& element_id,
        double& ref_coord ) const;

    // Given a reference coordinate in an element get the values of the shape
    // functions at that coordinat.
    void shapeFunctionValues( const double& ref_coord,
                             std::array<double,2>& values ) const;

    // Given reference coordinates in an element get the gradient of the shape
    // function at that coordinate. 
    void shapeFunctionDerivatives(
        const double& ref_coord,
        std::array<double, 2>& derivatives ) const;

    // Map a reference coordinate in an element from its local frame to
    // the global frame.
    void mapLocalToGlobalFrame(
        const double& ref_coord,
        const double& n1, const double& n2,
        double& x ) const;

    // Evaluate the approximate solution at x.
    double approxValue( const double& x,
                      const std::vector<double>& d,
                      const std::vector<Element>& elements );

  private:

    // Number of nodes. 
    int d_num_nodes;

    // Number of elements. 
    int d_num_elements;

    // Mesh celement width.
    double d_element_width;

    // Boundary nodes.
    std::vector<int> d_boundary_nodes;

};

} 

//---------------------------------------------------------------------------//

#endif // end FEA_MESH_HH

//---------------------------------------------------------------------------//
// end Mesh.hh
//---------------------------------------------------------------------------//
