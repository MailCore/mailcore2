package com.libmailcore;

import java.util.List;

public class NNTPListNewsgroupsOperation extends NNTPOperation {
    public native List<NNTPGroupInfo> groups();
}
