 
//snippet-sourcedescription:[<<FILENAME>> demonstrates how to ...]
//snippet-keyword:[C++]
//snippet-keyword:[Code Sample]
//snippet-keyword:[Amazon S3]
//snippet-service:[s3]
//snippet-sourcetype:[full-example]
//snippet-sourcedate:[]
//snippet-sourceauthor:[tapasweni-pathak]


﻿/*
Copyright 2010-2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.

This file is licensed under the Apache License, Version 2.0 (the "License").
You may not use this file except in compliance with the License. A copy of
the License is located at

http://aws.amazon.com/apache2.0/

This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
*/
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <fstream>

/**
* Put a Unicode file to an Amazon S3 bucket.
*/
int main(int argc, char** argv)
{
    Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Trace;
    Aws::InitAPI(options);
    {
        const Aws::String bucket_name = "unicode-test";

        #ifdef _WIN32
        const Aws::WString key_name = L"unicode-Ⓢ③Ⓢ③Ⓒⓛⓘ.txt";
        const Aws::WString file_name = L"Ⓢ③Ⓢ③Ⓒⓛⓘ.txt";
        #else
        const Aws::String key_name = "unicode-Ⓢ③Ⓢ③Ⓒⓛⓘ.txt";
        const Aws::String file_name = "Ⓢ③Ⓢ③Ⓒⓛⓘ.txt";
        #endif
        const Aws::String region = "us-west-2";

        std::cout << "Uploading to S3 bucket " <<
            bucket_name << std::endl;

        Aws::Client::ClientConfiguration clientConfig;
        clientConfig.region = region;
        Aws::S3::S3Client s3_client(clientConfig);

        Aws::S3::Model::PutObjectRequest object_request;
        object_request.SetBucket(bucket_name);
        #ifdef _WIN32
        object_request.SetKey(Aws::Utils::StringUtils::FromWString(key_name.c_str()));
        #else
        object_request.SetKey(key_name);
        #endif

        // Binary files must also have the std::ios_base::bin flag or'ed in
        auto input_data = Aws::MakeShared<Aws::FStream>("PutObjectInputStream",
            file_name.c_str(), std::ios_base::in | std::ios_base::binary);

        object_request.SetBody(input_data);

        auto put_object_outcome = s3_client.PutObject(object_request);

        if (put_object_outcome.IsSuccess())
        {
            std::cout << "Done!" << std::endl;
        }
        else
        {
            std::cout << "PutObject error: " <<
                put_object_outcome.GetError().GetExceptionName() << " " <<
                put_object_outcome.GetError().GetMessage() << std::endl;
        }
    }
    Aws::ShutdownAPI(options);
}

