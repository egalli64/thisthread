/**
 * Hello libcurl world
 *
 * Info: http://thisthread.blogspot.com/2013/09/hello-libcurl-world.html
 */

#include <curl/curl.h>
#include <iostream>

namespace
{
  size_t curlWriter(void* buf, size_t size, size_t nmemb)
  {
    if(std::cout.write(static_cast<char*>(buf), size * nmemb))
      return size * nmemb;
    return 0;
  }
}

CURLcode curling(const std::string& url, long timeout = 5, bool follow = false)
{
  CURL* curl = curl_easy_init();
  if(!curl)
    return CURLE_FAILED_INIT;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlWriter);
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
  if(follow)
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  CURLcode code = curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  return code;
}

int main()
{
  CURLcode result = curl_global_init(CURL_GLOBAL_NOTHING);
  if(result != CURLE_OK)
  {
    std::cout << "Curl initialization failure" << result << std::endl;
    return result;
  }

  result = curling("http://news.google.com/news?output=rss", 1);
  if(result != CURLE_OK)
  { // @curl.h#400 CURLE_OPERATION_TIMEDOUT 28
    std::cout << "Curl failure " << result << std::endl;
  }

  curl_global_cleanup();
  return result;
}
