//
//  MCMCNNTPFetchArticlesOperation.cpp
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#include "MCNNTPFetchArticlesOperation.h"

#include "MCNNTPAsyncSession.h"
#include "MCNNTPSession.h"

using namespace mailcore;

NNTPFetchArticlesOperation::NNTPFetchArticlesOperation()
{
    mGroupName = NULL;
    mArticles = NULL;
}

NNTPFetchArticlesOperation::~NNTPFetchArticlesOperation()
{
    MC_SAFE_RELEASE(mGroupName);
    MC_SAFE_RELEASE(mArticles);
}

void NNTPFetchArticlesOperation::setGroupName(String * groupname)
{
    MC_SAFE_REPLACE_COPY(String, mGroupName, groupname);
}

String * NNTPFetchArticlesOperation::groupName()
{
    return mGroupName;
}

IndexSet * NNTPFetchArticlesOperation::articles()
{
    return mArticles;
}

void NNTPFetchArticlesOperation::main()
{
    ErrorCode error;
    mArticles = session()->session()->fetchAllArticles(mGroupName, &error);
    setError(error);
    MC_SAFE_RETAIN(mArticles);
}
