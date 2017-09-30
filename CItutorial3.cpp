/***   CItutorial3.cpp   *****************************************************
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
#include "clang/Lex/HeaderSearch.h"
#include "clang/Frontend/Utils.h"

#include "clang/Lex/PreprocessorOptions.h"

void addSystemSearchPath(clang::CompilerInstance& ci, std::string path){
  const clang::DirectoryEntry * directory = ci.getFileManager().getDirectory(path);
  const clang::DirectoryLookup directory_lookup(directory, clang::SrcMgr::C_System, false);
  ci.getPreprocessor().getHeaderSearchInfo().AddSearchPath(directory_lookup, true);
}

int main()
{
  using clang::CompilerInstance;
  using clang::TargetOptions;
  using clang::TargetInfo;
  using clang::FileEntry;
  using clang::Token;
  using clang::HeaderSearch;
  using clang::HeaderSearchOptions;
  using clang::DiagnosticOptions;
  using clang::TextDiagnosticPrinter;

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
  ci.getPreprocessorOpts().UsePredefines = true;

  addSystemSearchPath(ci, "/usr/include");
  addSystemSearchPath(ci, "/usr/include/x86_64-linux-gnu/");
  addSystemSearchPath(ci, "/usr/include/x86_64-linux-gnu/c++/4.9/");
  addSystemSearchPath(ci, "/usr/include/linux/");
  addSystemSearchPath(ci, "/usr/include/c++/4.9/");
  addSystemSearchPath(ci, "/usr/include/c++/4.9/tr1/");
  addSystemSearchPath(ci, "/usr/include/c++/4.9/tr2/");

  clang::RawPCHContainerReader container_reader;
  clang::InitializePreprocessor(ci.getPreprocessor(),
                                ci.getPreprocessorOpts(),
                                container_reader,
                                ci.getFrontendOpts());

  const FileEntry *pFile = ci.getFileManager().getFile("testInclude.c");
  ci.getSourceManager().setMainFileID( ci.getSourceManager().createFileID( pFile, clang::SourceLocation(), clang::SrcMgr::C_User));
  ci.getPreprocessor().EnterMainSourceFile();
  ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                           &ci.getPreprocessor());
  Token tok;
  do {
    ci.getPreprocessor().Lex(tok);
    if( ci.getDiagnostics().hasErrorOccurred())
      break;
    ci.getPreprocessor().DumpToken(tok);
    std::cerr << std::endl;
  } while ( tok.isNot(clang::tok::eof));
  ci.getDiagnosticClient().EndSourceFile();

  return 0;
}
