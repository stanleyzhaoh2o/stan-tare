// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2026 stan All Rights Reserved.

#include <librealsense2/rs.hpp> // RealSense Cross Platform API
#include <iostream>

int main() try
{
    // Create a pipeline - this serves as a top-level API for streaming and processing frames
    rs2::pipeline pipe;
    
    // Create cfg and start the pipeline
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_DEPTH, 256, 144, RS2_FORMAT_Z16, 90);
    rs2::pipeline_profile profile = pipe.start(cfg);
    rs2::frameset data = pipe.wait_for_frames(); // Wait for streaming to start, will throw on timeout
    
    // Get depth frame
    rs2::depth_frame depth = data.get_depth_frame();
    auto stream_profile = depth.get_profile().as<rs2::video_stream_profile>();
    int width = stream_profile.width();
    int height = stream_profile.height();
    int x = width / 2;
    int y = height / 2;

    float ground_truth_mm = depth.get_distance(x, y) * 1000;
    std::cout << "ground_truth_mm value:" << ground_truth_mm << std::endl;
   
    // Control calibration parameters using a JSON input
    std::stringstream ss;
    ss << "{"
       << "\n \"average_step_count\":" << 20
       << ",\n \"step count\":" << 10
       << ",\n \"accuracy\":" << 0
       << ",\n \"scan parameter\":" << 0
       << ",\n \"data sampling\":" << 0
       << "\n}";
    std::string json = ss.str();
    std::cout << "Starting Tare with configuration:\n" << json << std::endl;

    // Get device with calibration API
    rs2::device dev = profile.get_device();  
    rs2::auto_calibrated_device cal_dev = dev.as< rs2::auto_calibrated_device >();
    float health;

    // Run calibration
    rs2::calibration_table res = cal_dev.run_tare_calibration(ground_truth_mm, json, &health, [&](const float progress) {
        std::cout << "progress = " << progress << "%" << std::endl;
    });
    std::cout << "Comppleted successfully" << std::endl;
    
    // Device is currently using calibration results, but they are not saved and will be lost after HW reset/power cycling.
    std::cout << std::endl << " Keep results? Yes/No" << std::endl;
    std::string keep;
    std::cin >> keep;
    for (auto & c : keep)
        c = tolower(c);
    if (keep == "y" || keep == "yes")
    {
        cal_dev.set_calibration_table(res);
        cal_dev.write_calibration();
        std::cout << "Results saved to flash" << std::endl;
    } 
    else
    {
        // To return using previous calibration parameters you can 'get_calibration_table' before calibrating and save
       // the old table back. Or just reset the camera to avoid flash writes
       std::cout << "Results not saved" << std::endl;
    }

    return EXIT_SUCCESS;
}

catch (const rs2::error & e)
{
    std::cerr << "Error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n  " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
