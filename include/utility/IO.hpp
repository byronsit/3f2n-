//
// Created by udi on 22-6-15.
//

#ifndef TRAD_INCLUDE_UTILITY_IO_HPP_
#define TRAD_INCLUDE_UTILITY_IO_HPP_


#include <exrio/exrio_opencv.h> //为了读取exr格式用的函数,对应的github为https://github.com/byronsit/exrio，下载编译安装即可


/**
 * @brief 给数据集提供IO接口,精确的读取1个数据
 * @todo 基本没写完，肯定都要TODO*/
class SNEIO{
 public:
  std::vector<cv::Mat> image;
  std::vector<std::string> channel_names;
  cv::Mat depth_image, range_image;
  cv::Mat gtx, gty, gtz, tmp;
  std::string path = "/home/udi/Desktop/normal/dataset/outdoor/shanghai/standard_exr/0001.exr";


  void SetPath(std::string path){
    this->path = path;
  }
  readexr_opencv(path, image, channel_names);
  for (int i = 0; i < channel_names.size(); ++i) {
    if (channel_names.at(i) == "Z") {
      depth_image = image.at(i);
    }

    if (channel_names.at(i) == "R") {
      gtx = image.at(i);
    }

    if (channel_names.at(i) == "G") {
      gty = image.at(i);
    }

    if (channel_names.at(i) == "B") {
      gtz = image.at(i);
    }
  }
  cv::merge(std::vector<cv::Mat>{gtx, gty, gtz}, tmp);
//if (need_show_gt) ShowNormal("gt", tmp);

  cv::Matx33f K(281.6000, 0.0, 256.0,
  0.0, 414.5777, 212.0,
  0.0, 0.0, 1.0);
  auto KK = K;

//考虑到计算的问题，减1是必须的
  if (1) {
    KK(0, 2)--;
    KK(1, 2)--;
  }

  if (depth_image.type() != CV_32FC1) {
    depth_image.convertTo(depth_image, depth_image.rows, depth_image.cols,
                          CV_32FC1);
  }



};


#endif //TRAD_INCLUDE_UTILITY_IO_HPP_
