
#include "FaceTree.h"

//
//  Triangle.cpp
//                          2009.06.19
//                          2008.12.01
//                          k.Takeda
#include "Triangle.h"
using namespace pmw;

uint CTriangle::mnElemType = ElementType::Triangle;
uint CTriangle::mnElemType2= ElementType::Triangle2;//2次要素
uint CTriangle::mNumOfFace = 1;
uint CTriangle::mNumOfEdge = 3;
uint CTriangle::mNumOfNode = 3;
//
//
CTriangle::CTriangle()
{
    mvNode.resize(mNumOfNode);
    mvInterNode.resize(mNumOfEdge + mNumOfFace);
    mvvNeibElement.resize(mNumOfEdge + mNumOfFace);

    uint i;
    for(i=mNumOfEdge; i < mNumOfEdge + mNumOfFace; i++){
        mvvNeibElement[i].resize(1);//Faceに隣接する要素は1個に限定
    }

    // Edge, Face bool
    //
    mvb_neib.reserve(mNumOfEdge + mNumOfFace);
    for(i=0; i < mNumOfEdge + mNumOfFace; i++){
        mvb_neib.push_back(false);
    }

    //CommElementのprolongation用
    mvProgElement.resize(mNumOfNode);


    //MPC属性
    mvbMPCFace.resize(mNumOfFace);
    for(i=0; i< mNumOfFace; i++){
        mvbMPCFace[i]= false;
    };

    //通信界面(CommMesh2)属性:辺
    mvbCommEntity.resize(mNumOfEdge);
    for(i=0; i< mNumOfEdge; i++){
        mvbCommEntity[i]= false;
    };
}

//
//
CTriangle::~CTriangle()
{
//    //debug
//    cout << "~CTriangle" << endl;
}


const uint& CTriangle::getType()
{
    return mnElemType;
}



bool CTriangle::IndexCheck(const uint& propType, const uint& index, string& method_name)
{
    Utility::CLogger *pLogger = Utility::CLogger::Instance();

    uint numOfProp;
    switch(propType){
        case(ElementPropType::Face):
            numOfProp = mNumOfFace;
            break;
        case(ElementPropType::Edge):
            numOfProp = mNumOfEdge;
            break;
        case(ElementPropType::Node):
            numOfProp = mNumOfNode;
            break;
        default:
            numOfProp = 0;
            break;
    }

    if(index >= numOfProp){
        pLogger->Info(Utility::LoggerMode::Error, "CTriangle::"+ method_name);
        return false;
    }else{
        return true;
    }
}



// out:(PariNode edgeNode)
//
PairNode& CTriangle::getPairNode(const uint& edgeIndex)
{
    switch(edgeIndex){
        //surf 0
        case(0):
            mEdgeNode.first=mvNode[0];
            mEdgeNode.second=mvNode[1];
            break;
        case(1):
            mEdgeNode.first=mvNode[1];
            mEdgeNode.second=mvNode[2];
            break;
        case(2):
            mEdgeNode.first=mvNode[2];
            mEdgeNode.second=mvNode[0];
            break;

        default:
            break;
    }

    return mEdgeNode;
}

// out:(vint& pairNodeIndex) -> pair Node index num.
//
void CTriangle::getPairNode(vint& pairNodeIndex, const uint& edgeIndex)
{
    switch(edgeIndex){
        //surf 0 (low surf)
        case(0):
            pairNodeIndex[0]= mvNode[0]->getID();
            pairNodeIndex[1]= mvNode[1]->getID();
            break;
        case(1):
            pairNodeIndex[0]= mvNode[1]->getID();
            pairNodeIndex[1]= mvNode[2]->getID();
            break;
        case(2):
            pairNodeIndex[0]= mvNode[2]->getID();
            pairNodeIndex[1]= mvNode[0]->getID();
            break;

        default:
            break;
    }
}

// ノードの局所番号に対応した辺(Edge)番号
//
uint& CTriangle::getEdgeIndex(CNode* pNode0, CNode* pNode1)
{ 
    getLocalNodeNum(pNode0, pNode1);//mvPairNodeLocalNumに値が入る.

    CEdgeTree *edgeTree = CEdgeTree::Instance();

    return edgeTree->getTriangleEdgeIndex(mvPairNodeLocalNum[0], mvPairNodeLocalNum[1]);//Triangle Edge Tree
}
uint& CTriangle::getEdgeIndex(const uint& nodeID_0, const uint& nodeID_1)
{
    mvPairNodeLocalNum[0]= mmIDLocal[nodeID_0];
    mvPairNodeLocalNum[1]= mmIDLocal[nodeID_1];

    CEdgeTree *pEdgeTree= CEdgeTree::Instance();

    return pEdgeTree->getTriangleEdgeIndex(mvPairNodeLocalNum[0], mvPairNodeLocalNum[1]);
}


// EdgeElement集合がセット済みか?
//
bool CTriangle::isEdgeElem(CNode* pNode0, CNode* pNode1)
{
    //    //
    //    //-- getLocalNodeNum(pNode0, pNode1)と同じ --
    //    //
    //    uint index0, index1;//MeshでのノードのIndex番号
    //    uint local0, local1;//局所ノード番号
    //
    //    index0= pNode0->getID();  index1= pNode1->getID();
    //
    //    // pNode0とpNode1のグローバルなIndex番号 => 局所番号に変換
    //    //
    //    uint i;
    //    for(i=0; i< mvNode.size(); i++){
    //        if(index0 == mvNode[i]->getID()) local0 = i;
    //        if(index1 == mvNode[i]->getID()) local1 = i;
    //    };
    //    //--------------------------------------------


    // Edgeに要素集合が作られているか? のbool値を返す
    uint edgeNum;
    edgeNum = getEdgeIndex(pNode0, pNode1);//Triangle Edge Tree

    return mvb_neib[edgeNum];
}

// EdgeElement集合がセットされていることをスタンプ
//
void CTriangle::setBoolEdgeElem(CNode* pNode0, CNode* pNode1)
{
    //vuint vLocalNum = getLocalNodeNum(pNode0, pNode1);

    // Edgeに要素集合が作られているか? のbool値を返す
    uint edgeNum;
    edgeNum = getEdgeIndex(pNode0, pNode1);//Triangle Edge Tree

    mvb_neib[edgeNum]=true;// スタンプ
}

// 局所ノード番号が正しければ"0"を返す.
//
uint& CTriangle::getLocalFaceNum(const vuint& vLocalNodeNum)
{
    CFaceTree *pFaceTree = CFaceTree::Instance();
    uint faceIndex = pFaceTree->getTriangleFaceIndex(vLocalNodeNum);

    if(faceIndex==0){
        mTempo= 0;
    }else{
        Utility::CLogger *pLogger = Utility::CLogger::Instance();
        pLogger->Info(Utility::LoggerMode::Error,"mismatch!, Local Node number@CTriangle::getLocalFaceNum");
        mTempo= 1;
    }
    return mTempo;
}

// 3個のノードから、面番号を取得.
//
uint& CTriangle::getFaceIndex(CNode* pNode0, CNode* pNode1, CNode* pNode2)
{
    // 面構成ノード -> 局所ノード番号
    // 局所ノード番号 -> 面番号==iface
    vuint vLocalNum; vLocalNum.resize(3);

    //vLocalNum[0]= mmIDLocal[pNode0->getID()];
    //vLocalNum[1]= mmIDLocal[pNode1->getID()];
    //vLocalNum[2]= mmIDLocal[pNode2->getID()];
    vLocalNum[0]= 0;
    vLocalNum[1]= 1;
    vLocalNum[2]= 2;

    CFaceTree *faceTree = CFaceTree::Instance();
    uint faceIndex = faceTree->getTriangleFaceIndex(vLocalNum);

    if(faceIndex==0){
        mTempo= 0;
    }else{
        Utility::CLogger *pLogger= Utility::CLogger::Instance();
        pLogger->Info(Utility::LoggerMode::Error,"mismatch!, Local Node Number @Triangle::getLocalFaceNum(arg:pNode)");
        mTempo= 1;
    }
    return mTempo;
}

// Face Element
//
void CTriangle::setFaceElement(CElement* pElem, const uint& faceIndex)
{
    uint index = mNumOfEdge + faceIndex;
    mvvNeibElement[index][0]= pElem;
}

void CTriangle::setFaceNode(CNode* pNode, const uint& faceIndex)
{
    uint index = mNumOfEdge + faceIndex;
    mvInterNode[index]= pNode;
}

CNode* CTriangle::getFaceNode(const uint& faceIndex)
{
    uint index = mNumOfEdge + faceIndex;
    return mvInterNode[index];
}

// 面(Face)に既に面ノードがセット済みかどうか
//
bool CTriangle::isFaceElem(CNode* pNode0, CNode* pNode1, CNode* pNode2)
{
    uint iface= getFaceIndex(pNode0,pNode1,pNode2);

    return mvb_neib[mNumOfEdge + iface];
}

// 面(Face)に面ノードがセットされたことをスタンプ
//
void CTriangle::setBoolFaceElem(CNode* pNode0, CNode* pNode1, CNode* pNode2)
{
    uint iface = getFaceIndex(pNode0,pNode1,pNode2);

    mvb_neib[mNumOfEdge + iface]=true;
}

// 2 Edge => Face Index
//
uint& CTriangle::getFaceIndex(const uint& edge0, const uint& edge1)
{
    CEdgeFaceTree *pTree= CEdgeFaceTree::Instance();

    return pTree->getTriangleFaceIndex(edge0, edge1);
}

// mvvFaceCnvNodeのセットアップ
//
void CTriangle::setupFaceCnvNodes()
{
    // Face構成のノード・コネクティビティ
    //
    CNode *pNode;
    mvvFaceCnvNode.resize(1);
    mvvFaceCnvNode[0].resize(3);
    uint ivert;

    for(ivert=0; ivert< 3; ivert++){
        pNode= mvNode[ivert];
        mvvFaceCnvNode[0][ivert]=pNode;
    };
}


// Node周囲の接続先Nodeを配列で返す.
// (係数行列 作成用途, CMesh::setupAggregate)
//
vector<CNode*> CTriangle::getConnectNode(CNode* pNode)
{
    vector<CNode*> vNode;
    vNode.reserve(2);

    uint gID= pNode->getID();
    uint localID= mmIDLocal[gID];

    CNodeConnectNodeTree *pConnTree = CNodeConnectNodeTree::Instance();
    vuint vLocalID;
    vLocalID= pConnTree->getTriangleConnectNode(localID);

    uint i;
    for(i=0; i< 2; i++){
        localID= vLocalID[i];
        vNode.push_back(mvNode[localID]);
    };

    return vNode;
}



// prolongation後の後始末
// --
void CTriangle::deleteProgData()
{
    CElement::deleteProgData();

    // 辺ノード クリア(ポインターはそのまま、削除しない)
    mvInterNode.clear();
}











