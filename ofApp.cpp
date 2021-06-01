#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);
	ofEnableDepthTest();

	this->ico_sphere = ofIcoSpherePrimitive(100, 2);
	this->noise_param = ofRandom(1000);
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 60 < 20) {

		this->noise_param += ofMap(ofGetFrameNum() % 60, 0, 20, 0.2, 0);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	ofSetColor(39);
	ofSetLineWidth(3);
	ofNoFill();

	auto tmp_noise_param = this->noise_param;

	ofBeginShape();
	for (int x = 0; x <= 330; x += 1) {

		if (x > 0 && (ofGetFrameNum() + x) % 60 < 20) {

			tmp_noise_param += ofMap((ofGetFrameNum() + x) % 60, 0, 20, 0.2, 0);
		}
		
		auto y = ofMap(ofNoise(tmp_noise_param), 0, 1, -100, 100);
		ofVertex(x, y);
	}
	ofEndShape();

	ofDrawLine(glm::vec2(0, -100), glm::vec2(0, 100));
	ofDrawLine(glm::vec2(330, -100), glm::vec2(330, 100));

	auto y = ofMap(ofNoise(this->noise_param), 0, 1, -100, 100);
	ofFill();
	ofDrawCircle(0, y, 8);

	ofTranslate(-180, 0);
	ofSetColor(39);
	ofSetLineWidth(1.5);

	ofRotateX(ofGetFrameNum() * 0.77);
	ofRotateY(ofGetFrameNum() * 0.39);

	auto triangle_list = this->ico_sphere.getMesh().getUniqueFaces();
	for (auto& triangle : triangle_list) {

		glm::vec3 avg = (triangle.getVertex(0) + triangle.getVertex(1) + triangle.getVertex(2)) / 3;
		auto noise_value = ofNoise(glm::vec4(avg * 0.015, this->noise_param));
		auto noise_radius = 0;

		if (noise_value < 0.65) {

			noise_radius = 80;
		}
		else {

			noise_radius = ofMap(noise_value, 0.65, 1.0, 80, 160);
		}

		ofMesh mesh;

		mesh.addVertex(glm::normalize(triangle.getVertex(0)) * noise_radius);
		mesh.addVertex(glm::normalize(triangle.getVertex(1)) * noise_radius);
		mesh.addVertex(glm::normalize(triangle.getVertex(2)) * noise_radius);

		avg = (mesh.getVertex(0) + mesh.getVertex(1) + mesh.getVertex(2)) / 3;

		ofSetColor(239);
		mesh.draw();

		ofSetColor(39);
		mesh.drawWireframe();

		ofDrawLine(glm::vec3(), avg);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}