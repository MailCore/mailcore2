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

MCNNTPFetchArticlesOperation::MCNNTPFetchArticlesOperation()
{
    mGroupName = NULL;
    mArticles = NULL;
}

MCNNTPFetchArticlesOperation::~MCNNTPFetchArticlesOperation()
{
    MC_SAFE_RELEASE(mGroupName);
    MC_SAFE_RELEASE(mArticles);
}

void MCNNTPFetchArticlesOperation::setGroupName(String * groupname)
{
    MC_SAFE_REPLACE_COPY(String, mGroupName, groupname);
}

String * MCNNTPFetchArticlesOperation::groupName()
{
    return mGroupName;
}

IndexSet * MCNNTPFetchArticlesOperation::articles()
{
    return mArticles;
}

void MCNNTPFetchArticlesOperation::main()
{
    ErrorCode error;
    mArticles = session()->session()->fetchArticles(mGroupName, &error);
    setError(error);
    MC_SAFE_RETAIN(mArticles);
}
