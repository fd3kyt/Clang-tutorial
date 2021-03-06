/***   CItutorial6.cpp   *****************************************************
 * This code is licensed under the New BSD license.
 * See LICENSE.txt for details.
 *
 * The CI tutorials remake the original tutorials but using the
 * CompilerInstance object which has as one of its purpose to create commonly
 * used Clang types.
 *****************************************************************************/
#include <iostream>

#include "llvm/Support/Host.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseAST.h"

#include "clang/Lex/PreprocessorOptions.h"

/******************************************************************************
 *
 *****************************************************************************/
class MyASTConsumer : public clang::ASTConsumer
{
 public:
  MyASTConsumer() : clang::ASTConsumer() { }
  virtual ~MyASTConsumer() { }

  virtual bool HandleTopLevelDecl( clang::DeclGroupRef d)
  {
    static int count = 0;
    clang::DeclGroupRef::iterator it;
    for( it = d.begin(); it != d.end(); it++)
    {
      count++;
      clang::VarDecl *vd = llvm::dyn_cast<clang::VarDecl>(*it);
      if(!vd)
      {
        continue;
      }
      if( vd->isFileVarDecl() && !vd->hasExternalStorage() )
      {
        std::cerr << "Read top-level variable decl: '";
        std::cerr << vd->getDeclName().getAsString() ;
        std::cerr << std::endl;
      }
    }
    return true;
  }
};

/******************************************************************************
 *
 *****************************************************************************/
int main()
{
  using clang::CompilerInstance;
  using clang::TargetOptions;
  using clang::TargetInfo;
  using clang::FileEntry;
  using clang::Token;
  using clang::ASTContext;
  using clang::ASTConsumer;
  using clang::Parser;
  using clang::DiagnosticOptions;
  using clang::TextDiagnosticPrinter;

  // #################### save as before (no HeaderSearch) ####################
  CompilerInstance ci;
  DiagnosticOptions diagnosticOptions;
  ci.createDiagnostics();

  std::shared_ptr<clang::TargetOptions> pto = std::make_shared<clang::TargetOptions>();
  pto->Triple = llvm::sys::getDefaultTargetTriple();
  TargetInfo *pti = TargetInfo::CreateTargetInfo(ci.getDiagnostics(), pto);
  ci.setTarget(pti);

  ci.createFileManager();
  ci.createSourceManager(ci.getFileManager());
  ci.createPreprocessor(clang::TU_Complete);
  ci.getPreprocessorOpts().UsePredefines = false;

  const FileEntry *pFile = ci.getFileManager().getFile("input04.c");
  ci.getSourceManager().setMainFileID( ci.getSourceManager().createFileID( pFile, clang::SourceLocation(), clang::SrcMgr::C_User));

  // #################### Parsing ####################
  ci.createASTContext();
  ci.setASTConsumer(llvm::make_unique<MyASTConsumer>());

  ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                           &ci.getPreprocessor());
  clang::ParseAST(ci.getPreprocessor(), &ci.getASTConsumer(), ci.getASTContext());
  ci.getDiagnosticClient().EndSourceFile();

  return 0;
}
