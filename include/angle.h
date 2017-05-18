#ifndef _ANGLE_H_
#define _ANGLE_H_

#include <sstream>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <button.h>

class AngleCircles {
public:
    AngleCircles(int pixel, float distance);
    ~AngleCircles();

    void update(float distance);
    float getBigCircleRadius() const;          // return distance(cm)
    float getSmallCircleRadius() const;        // return distance(cm)
    float getDistance() const;

    void setCircleRadius(float distance);   // set var distance

    void processEvents();

    // handle key input
    void handleInput(sf::Keyboard::Key key, bool isPressed);
    // process key press event
    void processPress();
    // process mouse wheel
    void processWheel(bool flag);
    // process mouse click event
    void processMouseButton(bool flag);
    // set button status
    void setButtonStatus(neo::ButtonStatus status);
    // get button status
    neo::ButtonStatus getButtonStatus();
    // get scanner status
    neo::Status getStatus();

    // draw the outline big circle
    void drawBigCircle();
    // draw the small circle
    void drawSmallCircle();
    // the draw function
    void draw();
    // show the scale the windows
    void showMarks();

private:
    int big_circle_radius_pixel_;   // big circle radius in pixel;
    float big_circle_radius_;       // big circle radius in centimeter;
    sf::CircleShape big_circle_;

    int small_circle_radius_pixel_; // small circle radius in pixel;
    float small_circle_radius_;     // small circle radius in meter;
    sf::CircleShape small_circle_;

    int radius_;
    sf::CircleShape small_circle_1_, big_circle_1_;

    int windows_size_;              // maximum distance in pixel
    float distance_;                // the maximum distance the windows can display.
                                    // range: [1-40] meter(there are the biggest circle's radius)

    // the text show on windows;
    sf::Text                     big_up_,

                                small_up_,

        big_left_, small_left_, center_, small_right_, big_right_,

                              small_bottom_,

                               big_bottom_;

    sf::Font text_font_;

    bool zoom_in_, /* Down */ zoom_out_ /* Up */;

    // for button
    neo::Button start_, pause_, stop_, help_;

    sf::RectangleShape outliers_start_; // start button outline
    sf::RectangleShape outliers_pause_; // pause button outline
    sf::RectangleShape outliers_stop_;  // stop button outline
    sf::RectangleShape outliers_help_;  // help button outline
    sf::RectangleShape outliers_left_;
    sf::RectangleShape outliers_right_;

    neo::Status status_;
    neo::ButtonStatus button_status_;

    neo::Button help_message_;
    bool show_help_;
public:
    sf::RenderWindow windows_;
};

// class for vertical and horizontal line
class AngleLines {
public:
    AngleLines(int pixel);
    ~AngleLines();

    void drawVerticalLine(sf::RenderWindow& windows);
    void drawHorizontalLine(sf::RenderWindow& windows);
    void draw(sf::RenderWindow& windows);

private:
    int windows_size_;
    // line vertical
    sf::RectangleShape vertical_line_;
    // line horizontal
    sf::RectangleShape horizontal_line_;
};

#endif // _ANGLE_H_
