//
//  MCIndexSet.cpp
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/4/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIndexSet.h"

#include "MCString.h"

using namespace mailcore;

void IndexSet::init()
{
    mRanges = NULL;
    mAllocated = 0;
    mCount = 0;
}

IndexSet::IndexSet()
{
    init();
}

IndexSet::IndexSet(IndexSet * o)
{
    init();
    mRanges = new Range[o->mAllocated];
    for(unsigned int i = 0 ; i < o->mCount ; i ++) {
        mRanges[i] = o->mRanges[i];
    }
    mAllocated = o->mAllocated;
    mCount = o->mCount;
}

IndexSet::~IndexSet()
{
    removeAllIndexes();
}

IndexSet * IndexSet::indexSet()
{
    IndexSet * result = new IndexSet();
    result->autorelease();
    return result;
}

unsigned int IndexSet::count()
{
    unsigned int total = 0;
    for(unsigned int i = 0 ; i < mCount ; i ++) {
        total += mRanges[i].length;
    }
    return total;
}

int IndexSet::rangeIndexForIndexWithBounds(uint64_t idx, unsigned int left, unsigned int right)
{
    unsigned int middle = (left + right) / 2;
    
    Range middleRange = mRanges[middle];
    
    if (left == right) {
        if ((idx >= middleRange.location) && (idx <= middleRange.location + middleRange.length)) {
            return left;
        }
        return -1;
    }

    if ((idx >= middleRange.location) && (idx <= middleRange.location + middleRange.length)) {
        return middle;
    }
    if (idx < middleRange.location) {
        return rangeIndexForIndexWithBounds(idx, left, middle);
    }
    else {
        return rangeIndexForIndexWithBounds(idx, middle + 1, right);
    }
}

int IndexSet::rangeIndexForIndex(uint64_t idx)
{
    if (mCount == 0)
        return -1;
    
    return rangeIndexForIndexWithBounds(idx, 0, mCount - 1);
}

int IndexSet::rightRangeIndexForIndexWithBounds(uint64_t idx, unsigned int left, unsigned int right)
{
    unsigned int middle = (left + right) / 2;
    
    Range middleRange = mRanges[middle];
    
    if (left == right) {
        if (idx > middleRange.location + middleRange.length) {
            return left + 1;
        }
        else {
            return left;
        }
    }
    
    if (idx < middleRange.location + middleRange.length) {
        return rightRangeIndexForIndexWithBounds(idx, left, middle);
    }
    else {
        return rightRangeIndexForIndexWithBounds(idx, middle + 1, right);
    }
}

int IndexSet::rightRangeIndexForIndex(uint64_t idx)
{
    if (mCount == 0)
        return 0;
    
    return rightRangeIndexForIndexWithBounds(idx, 0, mCount - 1);
}

void IndexSet::addRangeIndex(unsigned int rangeIndex)
{
    if (mAllocated < mCount + 1) {
        while (mAllocated < mCount + 1) {
            if (mAllocated == 0) {
                mAllocated = 4;
            }
            mAllocated *= 2;
        }
        
        Range * rangesReplacement = new Range[mAllocated];
        for(unsigned int i = 0 ; i < rangeIndex ; i ++) {
            rangesReplacement[i] = mRanges[i];
        }
        for(unsigned int i = rangeIndex ; i < mCount ; i ++) {
            rangesReplacement[i + 1] = mRanges[i];
        }
        rangesReplacement[rangeIndex].location = 0;
        rangesReplacement[rangeIndex].length = 0;
        delete [] mRanges;
        mRanges = rangesReplacement;
    }
    else {
        for(unsigned int i = mCount - 1 ; i >= rangeIndex ; i --) {
            mRanges[i + 1] = mRanges[i];
        }
        mRanges[rangeIndex].location = 0;
        mRanges[rangeIndex].length = 0;
    }
}

void IndexSet::addIndex(uint64_t idx)
{
    int leftRangeIndex;
    int rightRangeIndex;
    
    if (rangeIndexForIndex(idx) != -1)
        return;
    
    leftRangeIndex = rangeIndexForIndex(idx - 1);
    rightRangeIndex = rangeIndexForIndex(idx + 1);
    
    if ((leftRangeIndex == -1) && (rightRangeIndex == -1)) {
        rightRangeIndex = rightRangeIndexForIndex(idx);
        addRangeIndex(rightRangeIndex);
        mRanges[rightRangeIndex].location = idx;
        mRanges[rightRangeIndex].length = 0;
    }
    else if ((leftRangeIndex != -1) && (rightRangeIndex == -1)) {
        if (mRanges[leftRangeIndex].location + mRanges[leftRangeIndex].length + 1 == idx) {
            mRanges[leftRangeIndex].length ++;
        }
    }
    else if ((leftRangeIndex == -1) && (rightRangeIndex != -1)) {
        if (mRanges[rightRangeIndex].location - 1 == idx) {
            mRanges[rightRangeIndex].location --;
            mRanges[rightRangeIndex].length ++;
        }
    }
}

void IndexSet::removeRangeIndex(unsigned int rangeIndex)
{
    for(unsigned int i = rangeIndex + 1 ; i < mCount ; i --) {
        mRanges[i - 1] = mRanges[i];
    }
    mCount --;
}

void IndexSet::removeIndex(uint64_t idx)
{
    int rangeIndex = rangeIndexForIndex(idx);
    if (rangeIndex == -1)
        return;
    if (idx == mRanges[rangeIndex].location) {
        mRanges[rangeIndex].location ++;
        mRanges[rangeIndex].length --;
        if (mRanges[rangeIndex].length == 0) {
            // remove range.
            removeRangeIndex(rangeIndex);
        }
    }
    else if (idx == mRanges[rangeIndex].location + mRanges[rangeIndex].length) {
        mRanges[rangeIndex].length --;
        if (mRanges[rangeIndex].length == 0) {
            // remove range.
            removeRangeIndex(rangeIndex);
        }
    }
    else {
        // split range.
        addRangeIndex(rangeIndex);
        mRanges[rangeIndex] = mRanges[rangeIndex + 1];
        uint64_t right = mRanges[rangeIndex].location + mRanges[rangeIndex].length;
        mRanges[rangeIndex].location = mRanges[rangeIndex + 1].location;
        mRanges[rangeIndex].length = idx - mRanges[rangeIndex].location;
        mRanges[rangeIndex + 1].location = idx + 1;
        mRanges[rangeIndex + 1].length = right - mRanges[rangeIndex + 1].location;
    }
}

bool IndexSet::containsIndex(uint64_t idx)
{
    int rangeIndex = rangeIndexForIndex(idx);
    return rangeIndex != -1;
}

Range * IndexSet::allRanges()
{
    return mRanges;
}

void IndexSet::removeAllIndexes()
{
    if (mRanges != NULL) {
        delete[] mRanges;
        mRanges = NULL;
    }
    mAllocated = 0;
    mCount = 0;
}

String * IndexSet::description()
{
    String * result = String::string();
    for(unsigned int i = 0 ; i < mCount ; i ++) {
        if (i != 0) {
            result->appendUTF8Format(",");
        }
        result->appendUTF8Format("%llu-%llu",
                                 (unsigned long long) mRanges[i].location,
                                 (unsigned long long) (mRanges[i].location + mRanges[i].length));
    }
    return result;
}

Object * IndexSet::copy()
{
    return new IndexSet(this);
}
