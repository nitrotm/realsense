/******************************************************************************
 Copyright (c) 2016, Intel Corporation
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors
 may be used to endorse or promote products derived from this software without
 specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#pragma once
#ifndef REALSENSE_CAMERA_ZR300_NODELET_H
#define REALSENSE_CAMERA_ZR300_NODELET_H

#include <string>
#include <vector>
#include <condition_variable>

#include <dynamic_reconfigure/server.h>
#include <sensor_msgs/Imu.h>

#include <realsense_camera/zr300_paramsConfig.h>
#include <realsense_camera/IMUInfo.h>
#include <realsense_camera/GetIMUInfo.h>
#include <realsense_camera/base_nodelet.h>

namespace realsense_camera
{
class ZR300Nodelet: public realsense_camera::BaseNodelet
{
public:
  ~ZR300Nodelet();
  void onInit();

protected:
  // Member Variables.
  ros::ServiceServer get_imu_info_;
  boost::shared_ptr<dynamic_reconfigure::Server<realsense_camera::zr300_paramsConfig>> dynamic_reconf_server_;
  bool enable_imu_;
  std::string imu_frame_id_;
  std::string imu_optical_frame_id_;
  geometry_msgs::Vector3 imu_angular_vel_;
  geometry_msgs::Vector3 imu_linear_accel_;
  geometry_msgs::Vector3 imu_last_angular_vel_;
  geometry_msgs::Vector3 imu_last_linear_accel_;
  double imu_ts_;
  double prev_imu_ts_;
  ros::Publisher imu_publisher_;
  ros::Publisher imu_diff_publisher_;
  boost::shared_ptr<boost::thread> imu_thread_;
  std::function<void(rs::motion_data)> motion_handler_;
  std::function<void(rs::timestamp_data)> timestamp_handler_;
  std::mutex imu_mutex_;
  std::condition_variable imu_cv_;
  bool imu_changed_;

  rs_extrinsics color2ir2_extrinsic_;      // color frame is base frame
  rs_extrinsics color2fisheye_extrinsic_;  // color frame is base frame
  rs_extrinsics color2imu_extrinsic_;      // color frame is base frame

  // Member Functions.
  virtual void getParameters();
  virtual void advertiseTopics();
  virtual void advertiseServices();
  virtual bool getIMUInfo(realsense_camera::GetIMUInfo::Request & req, realsense_camera::GetIMUInfo::Response & res);
  virtual std::vector<std::string> setDynamicReconfServer();
  virtual void startDynamicReconfCallback();
  virtual void setDynamicReconfigDepthControlPreset(int preset);
  virtual std::string setDynamicReconfigDepthControlIndividuals();
  virtual void configCallback(realsense_camera::zr300_paramsConfig &config, uint32_t level);
  virtual void getCameraExtrinsics();
  virtual void publishStaticTransforms();
  virtual void publishDynamicTransforms();
  virtual void publishIMU();
  virtual void setStreams();
  virtual void setIMUCallbacks();
  virtual void setFrameCallbacks();
  std::function<void(rs::frame f)> fisheye_frame_handler_, ir2_frame_handler_;
  virtual void stopIMU();

  virtual void setDepthEnable(bool &enable_depth);
  virtual bool setPowerCameraService(realsense_camera::SetPower::Request & req,
      realsense_camera::SetPower::Response & res);
  virtual bool forcePowerCameraService(realsense_camera::ForcePower::Request & req,
      realsense_camera::ForcePower::Response & res);
};
}  // namespace realsense_camera
#endif  // REALSENSE_CAMERA_ZR300_NODELET_H
