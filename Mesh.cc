//---------------------------------------------------------------------------//
/*!
 * \file Mesh.cc
 */
//---------------------------------------------------------------------------//

#include "Mesh.hh"
#include <cassert>
#include <cmath>
#include <iostream>

namespace FEA
{

//---------------------------------------------------------------------------//
// Constructor.
Mesh::Mesh( const int n )
    : d_num_nodes( n+1 )
    , d_num_elements( n )
    , d_element_width( 1./n )
{ 
    // Create the boundary nodes.
    
    d_boundary_nodes.resize(1);
    d_boundary_nodes[0] = d_num_nodes;
    
}

//---------------------------------------------------------------------------//
// Get the total number of elements in the mesh.
int Mesh::totalNumElements() const
{
    return d_num_elements;
}

//---------------------------------------------------------------------------//
// Get the total number of nodes in the mesh.
int Mesh::totalNumNodes() const
{
    return d_num_nodes;
}

//---------------------------------------------------------------------------//
// Get the width of each element in the mesh.
double Mesh::getElementWidth() const
{
    return d_element_width;
}

//---------------------------------------------------------------------------//
// Given a node id get its coordinate.
void Mesh::nodeCoordinate( const int node_id,
                            double& coord ) const
{
    assert( node_id < totalNumNodes() );

    // Get the coordinate.
    coord = node_id * d_element_width;
}

//---------------------------------------------------------------------------//
// Given an element id get its left and right endpoints. 
void Mesh::elementCoordinates( const int element_id,
                              std::array<double,2>& coords ) const
{
    assert( element_id < d_num_elements );

    // Initialize the coordinates.
    double n1;
    double n2;

    nodeCoordinate(element_id, n1);
    nodeCoordinate(element_id+1, n2);

    coords[0] = n1;
    coords[1] = n2;
}

//---------------------------------------------------------------------------//
// Given a cardinal element id intitalize the element.
void Mesh::initializeElement(
    const int element_id,
    Element& element ) const
{
    
    // Save node and element ids
    std::array<double,2> coords;
    elementCoordinates( element_id, coords );

    element.id = element_id;
    element.n1 = coords[0];
    element.n2 = coords[1];
    element.ind1 = element_id;
    element.ind2 = element_id + 1;
    
    // Compute local K on the element
    double h = element.n2 - element.n1;
    double val = ((element.n2 == 1.) ? 0. : 1. );
    element.k[0][0] = 1. / h;
    element.k[0][1] = -val / h;
    element.k[1][0] = -val / h;
    element.k[1][1] = val / h;
}

//---------------------------------------------------------------------------//
// Given a point in the domain, locate the cardinal index of the element
// in which it is located
void Mesh::locateX( const double& x,
                    int& element_id ) const
{
    // Mesh spans (0.0, 1.0)
    element_id = std::floor( x / d_element_width );
    if ( x == 1. )
    {
        element_id = d_num_elements - 1;
    }

    assert( 0 <= element_id );
    assert( element_id < d_num_elements );
}

//---------------------------------------------------------------------------//
// Map the value x from the global frame to the local frame of the element
// in which it is located.
void Mesh::mapGlobalToLocalFrame(
    const double& x,
    const int& element_id,
    double& ref_coord ) const
{
    assert( element_id == std::floor( x / d_element_width ) || element_id == (d_num_elements-1));

    // The reference cell spans -1 to 1 in the x direction 
    // and 0 to 1 in the y direction.
    //
    //     (-1,1)------------(1,1) 
    //       |                |
    //       |                |
    //       |                |
    //       |                |
    //     (-1,0)-----------(1,0)  

    ref_coord = ( x / d_element_width - element_id ) * 2. - 1.;

    assert( -1.0 <= ref_coord && ref_coord <= 1.0 );
}

//---------------------------------------------------------------------------//
// Given reference coordinate in a cell get the value of the shape
// functions at that coordinate.
void Mesh::shapeFunctionValues( const double& ref_coord,
                               std::array<double,2>& values ) const
{
    values[0] = (1.0 - ref_coord) / 2.;
    values[1] = (1.0 + ref_coord) / 2.;
}

//---------------------------------------------------------------------------//
// Given reference coordinates in an element get the gradient of the shape
// function at that coordinate.
void Mesh::shapeFunctionDerivatives(
    const double& ref_coord,
    std::array<double,2>& derivatives ) const
{
        derivatives[0] = -d_num_elements;
        derivatives[1] = d_num_elements;
}

//---------------------------------------------------------------------------//
// Evaluate the approximate solution at x.
double Mesh::approxValue( const double& x,
                        const std::vector<double>& d,
                        const std::vector<Element>& elements )
{
    int element_id;
    double ref_coord;
    std::array<double,2> values;

    locateX( x, element_id );
    mapGlobalToLocalFrame( x, element_id, ref_coord );
    shapeFunctionValues( ref_coord, values );

    auto& e = elements[ element_id ];

    return d[e.ind1] * values[0] + d[e.ind2] * values[1];
}

//---------------------------------------------------------------------------//
// Map a reference coordinate in an element from its local frame to 
// the global frame.
void Mesh::mapLocalToGlobalFrame(
    const double& ref_coord,
    const double& n1, const double& n2,
    double& x ) const
{
    x = ( ( 1. - ref_coord ) * n1 + ( 1. + ref_coord ) * n2 ) / 2.;
}
//---------------------------------------------------------------------------//

}

//---------------------------------------------------------------------------//
// end Mesh.cc
//---------------------------------------------------------------------------//
