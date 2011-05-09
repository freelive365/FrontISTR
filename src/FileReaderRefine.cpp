//
//  FileReaderRefine.cpp
//
//                  2009.04.20
//                  2009.04.08
//                  k.Takeda
#include "FileReaderRefine.h"
using namespace FileIO;

// 
//
CFileReaderRefine::CFileReaderRefine()
{
    ;
}

CFileReaderRefine::~CFileReaderRefine()
{
    ;
}

// マルチグリッド数ぶんのAssyModelを確保
//
bool CFileReaderRefine::Read(ifstream& ifs, string& sLine)
{
    uint numOfRefine;// refine段数

    if(TagCheck(sLine, FileBlockName::StartRefine()) ){

        // Refine数::マルチグリッド・レベル
        while(true){
            sLine = getLineSt(ifs);
            if(TagCheck(sLine, FileBlockName::EndRefine()) ) break;

            istringstream iss(sLine.c_str());
            // iss -> Token 
            iss >> numOfRefine;

            mpLogger->Info(Utility::LoggerMode::MWDebug, "FileReaderRefine::numOfRefine == ", numOfRefine);
        };

        // AssyModel  generate
        //
        mpFactory->GeneAssyModel(numOfRefine+1);//"Refine段数+1" => マルチグリッド階層数に合わせてAssyModelを生成
        mpFactory->setMGLevel(numOfRefine);     // Refine段数のセット


        return true;
    }else{
        return false;
    }
}