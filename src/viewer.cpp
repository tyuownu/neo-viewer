// Make use of the CMake build system or compile manually, e.g. with:
// g++ -std=c++11 viewer.cc -lneo -lsfml-graphics -lsfml-window -lsfml-system

#include <cmath>

#include <cstdlib>
#include <iostream>
#include <mutex>
#include <utility>

// #include <neo/neo.hpp>


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <angle.h>
#include <button.h>
#include <color.h>
#include <pointcloud.h>


int main(int argc, char* argv[]) try {
	if (argc != 2) {
		std::cout << "Usage: ./neo_viewer /dev/ttyACM0\n";
		return EXIT_FAILURE;
	}
	PointCloud pointCloud;
	PointCloudMutex pointCloudMutex;

    // motor shows
    sf::Font motor_font;
    motor_font.loadFromFile("fonts/Sansation.ttf");
    sf::Text motor_text;
    motor_text.setFont(motor_font);
    motor_text.setPosition(650, 5);
    motor_text.setCharacterSize(14);
    motor_text.setColor(sf::Color::Green);

	// Render thread displays the point cloud
    neo::AngleCircles circles(800, 16*100.);
    neo::AngleLines lines(800);
    motor_text.setString("Motor Speed: " + toString(circles.getMotorSpeed()));

	// Now start scanning in the second thread, swapping in new points for every scan
	neo::neo device{argv[1]};

	neo::scan scan;
    std::cout << "Device connect successful" << std::endl;

    sf::Clock clock;
    sf::Time time_since_last_update = sf::Time::Zero;

    while (circles.windows_.isOpen()) {
        sf::Time elapsed_time = clock.restart();
        time_since_last_update += elapsed_time;

        motor_text.setString("Motor Speed: " + toString(circles.getMotorSpeed()));
        // windows render frequency
        while (time_since_last_update > TimePreFrame) {
            time_since_last_update -= TimePreFrame;
            circles.processEvents();
            circles.windows_.clear(kColorSlateBlue4);
            {
                std::lock_guard<PointCloudMutex> sentry{pointCloudMutex};
                circles.draw();
                lines.draw(circles.windows_);
                circles.windows_.draw(motor_text);
                for (auto point : pointCloud)
                    circles.windows_.draw(point.point);
            }
            circles.windows_.display();
        }
        // deal with press button situation
        if (circles.getButtonStatus() == neo::ButtonStatus::BUTTON_START) {
            static bool first_start = true;
            // std::cout << "start running" << std::endl;
            circles.setButtonStatus(neo::ButtonStatus::BUTTON_NONE);
            if (first_start) {
                device.set_motor_speed(circles.getMotorSpeed());
                // device.set_motor_speed(5);
                device.start_scanning();
                first_start = false;
            } else
                device.start_scanning();
        } else if (circles.getButtonStatus() == neo::ButtonStatus::BUTTON_PAUSE) {
            // std::cout << "pause, and keep the pointcloud";
            circles.setButtonStatus(neo::ButtonStatus::BUTTON_NOT_RUN);
            continue;
        } else if (circles.getButtonStatus() == neo::ButtonStatus::BUTTON_STOP) {
            // std::cout << "stop and reset the scanner" << std::endl;
            circles.setButtonStatus(neo::ButtonStatus::BUTTON_NOT_RUN);
            pointCloud.clear();
            device.stop_scanning();
            continue;
        } else if (circles.getButtonStatus() == neo::ButtonStatus::BUTTON_HELP) {
            // std::cout << "Open the help window" << std::endl;
            circles.setButtonStatus(neo::ButtonStatus::BUTTON_NOT_RUN);
            continue;
        } else if (circles.getButtonStatus() == neo::ButtonStatus::BUTTON_NOT_RUN) {
            // std::cout << "Not_run" << std::endl;
            buildPointCloudWhenPause(circles, pointCloudMutex, pointCloud);
            continue;
        }
        // difference between BUTTON_NOT_RUN and BUTTON_NONE
        // BUTTON_NOT_RUN: the scanning stopped.
        // BUTTON_NONE: the scanner running and send data.

		scan = device.get_scan();
        buildPointCloud(scan, circles, pointCloudMutex, pointCloud);
    }
	device.stop_scanning();

} catch (const neo::device_error& e) {
	std::cerr << "Error: " << e.what() << std::endl;
}
