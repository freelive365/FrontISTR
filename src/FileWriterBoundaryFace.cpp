
#include "BoundaryMesh.h"

//
//  FileWriterBoundaryFace.cpp
//
//              2010.05.07
//              k.Takeda
#include "BoundaryNode.h"
#include "BoundaryFace.h"
#include "BoundaryFaceMesh.h"
#include "FileWriterBoundaryFace.h"
using namespace FileIO;


CFileWriterBoundaryFace::CFileWriterBoundaryFace()
{
    ;
}
CFileWriterBoundaryFace::~CFileWriterBoundaryFace()
{
    ;
}

void CFileWriterBoundaryFace::Write(ofstream& ofs, const uint& mgLevel)
{
   pmw::CAssyModel *pAssyModel;
   pmw::CMesh *pMesh;

   pmw::CBoundaryFaceMesh *pBndFaceMesh;
   pmw::CBoundaryFace *pBndFace;
   pmw::CBoundaryNode *pBndNode;
   pmw::CNode *pNode;

   pAssyModel= mpGMGModel->getAssyModel(mgLevel);

   uint i,ii,iii;
   uint numOfPart= pAssyModel->getNumOfMesh();

   for(i=0; i < numOfPart; i++){
       pMesh= pAssyModel->getMesh(i);
       
       uint numOfBndMesh= pMesh->getNumOfBoundaryFaceMesh();
       
       for(ii=0; ii < numOfBndMesh; ii++){
           pBndFaceMesh= pMesh->getBndFaceMeshIX(ii);

           ofs << "BoundaryID= " << pBndFaceMesh->getID() << ", BndType= ";
           switch(pBndFaceMesh->getBndType()){
               case(pmw::BoundaryType::Dirichlet):
                   ofs << "Dirichlet";
                   break;
               case(pmw::BoundaryType::Neumann):
                   ofs << "Neumann";
                   break;
               default:
                   //TODO:pLogger
                   break;
           }
           ofs << endl;

           uint numOfBndNode= pBndFaceMesh->getNumOfBNode();
           for(iii=0; iii < numOfBndNode; iii++){

               pBndNode= pBndFaceMesh->getBNodeIX(iii);
               pNode= pBndNode->getNode();
               
               ofs << "BNodeID= " << pBndNode->getID() << ", NodeID= " << pNode->getID();

               uint idof, numOfDOF= pBndFaceMesh->getNumOfDOF();
               for(idof=0; idof < numOfDOF; idof++){

                   uint dof = pBndFaceMesh->getDOF(idof);

                   ofs << ", dof= " << dof << ", Value[" << idof << "]= " << pBndNode->getValue(dof, mgLevel);
               };
               ofs << endl;
           };

           uint numOfBndFace= pBndFaceMesh->getNumOfBFace();
           for(iii=0; iii < numOfBndFace; iii++){
               pBndFace= pBndFaceMesh->getBFaceIX(iii);

               ofs << "BFaceID= " << pBndFace->getID();

               uint idof, numOfDOF= pBndFaceMesh->getNumOfDOF();
               for(idof=0; idof < numOfDOF; idof++){

                   uint dof = pBndFaceMesh->getDOF(idof);

                   ofs << ", Value[" << idof << "]= " << pBndFace->getBndValue(dof);
               };
               ofs << endl;
           };
       };
   };
}





