// This code is licensed under the New BSD license.
// See LICENSE.txt for more details.
#include <iostream>

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Host.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"

#include "clang/Basic/LangOptions.h"
#include "clang/Basic/FileSystemOptions.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Basic/FileManager.h"

#include "clang/Frontend/Utils.h"

#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"

#include "clang/Lex/Preprocessor.h"
#include "clang/Frontend/FrontendOptions.h"
#include "clang/Frontend/CompilerInstance.h"

#include "clang/Basic/MemoryBufferCache.h"
#include "clang/Lex/PreprocessorOptions.h"


int main()
{
  clang::DiagnosticOptions diagnosticOptions;
  clang::TextDiagnosticPrinter *pTextDiagnosticPrinter =
      new clang::TextDiagnosticPrinter(
          llvm::outs(),
          &diagnosticOptions);
  llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> pDiagIDs;
  clang::DiagnosticsEngine *pDiagnosticsEngine =
      new clang::DiagnosticsEngine(pDiagIDs,
                                   &diagnosticOptions,
                                   pTextDiagnosticPrinter);

  clang::LangOptions languageOptions;
  clang::FileSystemOptions fileSystemOptions;
  clang::FileManager fileManager(fileSystemOptions);

  clang::SourceManager sourceManager(
      *pDiagnosticsEngine,
      fileManager);

  const std::shared_ptr<clang::TargetOptions> targetOptions = std::make_shared<clang::TargetOptions>();
  targetOptions->Triple = llvm::sys::getDefaultTargetTriple();

  clang::TargetInfo *pTargetInfo = 
      clang::TargetInfo::CreateTargetInfo(
          *pDiagnosticsEngine,
          targetOptions);
  std::shared_ptr<clang::HeaderSearchOptions> hso(new clang::HeaderSearchOptions());

  clang::HeaderSearch headerSearch(hso,
                                   sourceManager, 
                                   *pDiagnosticsEngine,
                                   languageOptions,
                                   pTargetInfo);
  clang::CompilerInstance compInst;

  std::shared_ptr<clang::PreprocessorOptions> pOpts(new clang::PreprocessorOptions);
  clang::MemoryBufferCache memory_buffer_cache;
  
  clang::Preprocessor preprocessor(
      pOpts,
      *pDiagnosticsEngine,
      languageOptions,
      sourceManager,
      memory_buffer_cache,
      headerSearch,
      compInst);

  preprocessor.Initialize(*pTargetInfo);

  // disable predefined Macros so that you only see the tokens from your 
  // source file. Note, this has some nasty side-effects like also undefning
  // your archictecture and things like that.
  //preprocessorOptions.UsePredefines = false;

  clang::RawPCHContainerReader container_reader;

  clang::FrontendOptions frontendOptions;
  clang::InitializePreprocessor(
      preprocessor,
      *pOpts,
      container_reader,
      frontendOptions);
  clang::ApplyHeaderSearchOptions( preprocessor.getHeaderSearchInfo(),
                                   compInst.getHeaderSearchOpts(),
                                   preprocessor.getLangOpts(),
                                   preprocessor.getTargetInfo().getTriple());

  // Note: Changed the file from tutorial2.
  const clang::FileEntry *pFile = fileManager.getFile("testInclude.c");
  sourceManager.setMainFileID( sourceManager.createFileID( pFile, clang::SourceLocation(), clang::SrcMgr::C_User));
  preprocessor.EnterMainSourceFile();
  pTextDiagnosticPrinter->BeginSourceFile(languageOptions, &preprocessor);

  clang::Token token;
  do {
    preprocessor.Lex(token);
    if( pDiagnosticsEngine->hasErrorOccurred())
    {
      break;
    }
    preprocessor.DumpToken(token);
    std::cerr << std::endl;
  } while( token.isNot(clang::tok::eof));
  pTextDiagnosticPrinter->EndSourceFile();

  return 0;
}
