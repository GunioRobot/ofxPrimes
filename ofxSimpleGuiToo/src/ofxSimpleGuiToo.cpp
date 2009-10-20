/***********************************************************************

 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.

 based on Todd Vanderlin's ofxSimpleGui API
 http://toddvanderlin.com/

 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/

#include "ofxSimpleGuiToo.h"

//------------------------------------------------------------------------------ constrcutor
ofxSimpleGuiToo::ofxSimpleGuiToo() {
	config = NULL;
}

void ofxSimpleGuiToo::setup() {
	config			= &defaultSimpleGuiConfig;

	config->setup();
//	guiFocus		= 0;

	verbose			= true;

	xmlFile			= OFX_SIMPLEGUITOO_XML_NAME;
	doSave			= false;
	doSaveBackup	= false;
	changePage		= false;

	headerPage		= &addPage("Header");
	
	headerPage->addToggle("Auto Save", doAutoSave);
	headerPage->addButton("Save Settings", doSave);
	headerPage->addButton("Backup XML", doSaveBackup);

	headerPage->addFPSCounter();

	headerPage->height = config->buttonHeight * 2;
	headerPage->width = ofGetWidth();

	addPage();
	setAutoSave(false);
	setAlignRight(false);
	setDraw(true);
	setPage(1);
}



void ofxSimpleGuiToo::addListeners() {
//	ofAddListener(ofEvents.setup, this, &ofxSimpleGuiToo::setup);
	ofAddListener(ofEvents.update, this, &ofxSimpleGuiToo::update);
//	ofAddListener(ofEvents.draw, this, &ofxSimpleGuiToo::draw);
//	ofAddListener(ofEvents.exit, this, &ofxSimpleGuiToo::exit);

	ofAddListener(ofEvents.mousePressed, this, &ofxSimpleGuiToo::mousePressed);
	ofAddListener(ofEvents.mouseMoved, this, &ofxSimpleGuiToo::mouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &ofxSimpleGuiToo::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &ofxSimpleGuiToo::mouseReleased);

	ofAddListener(ofEvents.keyPressed, this, &ofxSimpleGuiToo::keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &ofxSimpleGuiToo::keyReleased);
}


void ofxSimpleGuiToo::removeListeners() {
//	ofRemoveListener(ofEvents.setup, this, &ofxSimpleGuiToo::setup);
	ofRemoveListener(ofEvents.update, this, &ofxSimpleGuiToo::update);
//	ofRemoveListener(ofEvents.draw, this, &ofxSimpleGuiToo::draw);
//	ofRemoveListener(ofEvents.exit, this, &ofxSimpleGuiToo::exit);

	ofRemoveListener(ofEvents.mousePressed, this, &ofxSimpleGuiToo::mousePressed);
	ofRemoveListener(ofEvents.mouseMoved, this, &ofxSimpleGuiToo::mouseMoved);
	ofRemoveListener(ofEvents.mouseDragged, this, &ofxSimpleGuiToo::mouseDragged);
	ofRemoveListener(ofEvents.mouseReleased, this, &ofxSimpleGuiToo::mouseReleased);

	ofRemoveListener(ofEvents.keyPressed, this, &ofxSimpleGuiToo::keyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &ofxSimpleGuiToo::keyReleased);
}

void ofxSimpleGuiToo::setDraw(bool b) {
	if(doDraw != b) {
		doDraw = b;
		if(doDraw) addListeners();
		else removeListeners();
		if(doAutoSave) saveToXML(xmlFile);
	}
}

void ofxSimpleGuiToo::toggleDraw() {
	setDraw(!doDraw);
}

bool ofxSimpleGuiToo::isOn() {
	return doDraw;
}


void ofxSimpleGuiToo::setAutoSave(bool b) {
	doAutoSave = b;
}


void ofxSimpleGuiToo::loadFromXML(string file) {
	if(verbose) printf("ofxSimpleGuiToo::loadFromXML( %s )\n", file.c_str());

	if(file.compare("NULL") != 0) xmlFile = file;
	else {
		xmlFile = OFX_SIMPLEGUITOO_XML_NAME;
	}

	if(XML.loadFile(xmlFile) == false) {
		if(verbose) printf("Error loading XMLFile: %s\n", xmlFile.c_str());
		return;
	}

	doDraw		= XML.getValue("options:doDraw", true);
	doAutoSave	= XML.getValue("options:doAutoSave", false);
	currentPage	= XML.getValue("options:currentPage", 1);

	for(int i=1; i < pages.size(); i++) {
		pages[i]->loadFromXML(XML);
	}

	setPage(currentPage);
	setDraw(doDraw);
}


void ofxSimpleGuiToo::saveToXML(string file) {
	doSave = false;

	XML.clear();	// clear cause we are building a new xml file

	XML.addTag("options");
	XML.pushTag("options");
	XML.addValue("doDraw", doDraw);
	XML.addValue("doAutoSave", doAutoSave);
	XML.addValue("currentPage", currentPage);
	XML.popTag();

	XML.addTag("controls");
	XML.pushTag("controls");
	for(int i=1; i < pages.size(); i++) {
		pages[i]->saveToXML(XML);
	}
	XML.popTag();

	XML.saveFile(file);
	if(doSaveBackup) XML.saveFile(file+".bak");
	printf("ofxSimpleGuiToo::saveToXML( %s )\n", file.c_str());
}


void ofxSimpleGuiToo::setVerbose(bool v) {
	verbose = v;
}

void ofxSimpleGuiToo::setAlignRight(bool b) {
	alignRight = b;
}


//
//int ofxSimpleGuiToo::getValueI(string nameID) {
//	for(int i=0; i<controls.size(); i++) {
//		if(nameID.compare(controls[i]->name) == 0) {
//			ofxSimpleGuiSliderInt *c = dynamic_cast<ofxSimpleGuiSliderInt *> (controls[i]);
//			if(c) return c->getValue();
//		}
//	}
//	return NULL;
//}
//
//float ofxSimpleGuiToo::getValueF(string nameID) {
//	for(int i=0; i<controls.size(); i++) {
//		if(nameID.compare(controls[i]->name) == 0) {
//			ofxSimpleGuiSliderFloat *c = dynamic_cast<ofxSimpleGuiSliderFloat *> (controls[i]);
//			if(c) return c->getValue();
//		}
//	}
//	return NULL;
//}
//
//bool ofxSimpleGuiToo::getValueB(string nameID) {
//	for(int i=0; i<controls.size(); i++) {
//		if(nameID.compare(controls[i]->name) == 0) {
//			ofxSimpleGuiToggle *c = dynamic_cast<ofxSimpleGuiToggle *> (controls[i]);
//			if(c) return c->getValue();
//		}
//	}
//	return NULL;
//}


void ofxSimpleGuiToo::drawFocus(float x, float y) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	ofFill();
//	ofSetColor(config->focusColor.r, config->focusColor.g, config->focusColor.b, 200);
	ofRect(0, 0, 10, 10);
	glPopMatrix();
}


void ofxSimpleGuiToo::draw() {
	if(!doDraw) return;

	glDisableClientState(GL_COLOR_ARRAY);

	headerPage->draw(config->offset.x, config->offset.y, alignRight);		// this is the header
	ofSetColor(config->borderColor);
	if(alignRight) ofLine(ofGetWidth() - headerPage->width, headerPage->height, headerPage->width, headerPage->height);
	else ofLine(0, headerPage->height, headerPage->width, headerPage->height);
	pages[currentPage]->draw(config->offset.x, headerPage->height + config->offset.y, alignRight);
}


void ofxSimpleGuiToo::nextPage() {
	setPage(currentPage + 1);
}
void ofxSimpleGuiToo::prevPage() {
	setPage(currentPage - 1);
}

void ofxSimpleGuiToo::nextPageWithBlank() {
	if(doDraw) {
		setPage(currentPage + 1);
		if(currentPage == 1) setDraw(false);
	} else {
		setDraw(true);
		setPage(1);
	}
}



void ofxSimpleGuiToo::setPage(int i) {
	currentPage = i;
	if(currentPage >= pages.size()) currentPage = 1;
	else if(currentPage < 1) currentPage = pages.size()-1;
}


void ofxSimpleGuiToo::setPage(string name) {
//	ofxSimpleGuiPage *page;
	for(int i=1; i < pages.size(); i++) {
		if(name.compare(pages[i]->name) == 0) {
			setPage(i);
			break;
		}
	}
}


ofxSimpleGuiPage &ofxSimpleGuiToo::page(int i) {
	return *pages.at(i);
}

ofxSimpleGuiPage &ofxSimpleGuiToo::page(string name) {
	if(!config) setup();

//	ofxSimpleGuiPage *page;
	for(int i=1; i<pages.size(); i++) if(name.compare(pages[i]->name) == 0) return *pages[i];
//	return NULL;
}


ofxSimpleGuiPage &ofxSimpleGuiToo::addPage(string name) {
	if(!config) setup();

	ofxSimpleGuiPage *newPage = new ofxSimpleGuiPage(ofToString(pages.size(), 0) + ": " + name);
	pages.push_back(newPage);
	if(name == "") newPage->setName(ofToString(pages.size()-1, 0) + ": Settings");
	static bool b;
//	if(pages.size() > 1) headerPage->addTitle(newPage->name);		// if this isn't the first page, add to header
	if(pages.size() > 1) newPage->addButton(newPage->name, changePage);		// if this isn't the first page, add to header
	setPage(pages.size() - 1);
	return *newPage;
}


ofxSimpleGuiControl &ofxSimpleGuiToo::addControl(ofxSimpleGuiControl& control) {
	if(!config) setup();
	return pages[currentPage]->addControl(control);
}

ofxSimpleGuiButton &ofxSimpleGuiToo::addButton(string name, bool &value) {
	if(!config) setup();
	return pages[currentPage]->addButton(name, value);
}

ofxSimpleGuiContent &ofxSimpleGuiToo::addContent(string name, ofBaseDraws &content, float fixwidth) {
	if(!config) setup();
	return pages[currentPage]->addContent(name, content, fixwidth);
}

ofxSimpleGuiFPSCounter &ofxSimpleGuiToo::addFPSCounter() {
	if(!config) setup();
	return pages[currentPage]->addFPSCounter();
}

//ofxSimpleGuiQuadWarp &ofxSimpleGuiToo::addQuadWarper(string name, float x, float y, float sw, float sh, ofPoint &&pts) {
//	if(!config) setup();
//  	return pages[currentPage]->addQuadWarper(name, x, y, sw, sh, pts);
//}

#ifndef TARGET_OF_IPHONE
ofxSimpleGuiMovieSlider &ofxSimpleGuiToo::addMovieSlider(string name, ofVideoPlayer* input) {
	if(!config) setup();
	return pages[currentPage]->addMovieSlider(name, input);
}
#endif

ofxSimpleGuiSliderInt &ofxSimpleGuiToo::addSlider(string name, int &value, int min, int max) {
	if(!config) setup();
	return pages[currentPage]->addSlider(name, value, min, max);
}

ofxSimpleGuiSliderByte &ofxSimpleGuiToo::addSlider(string name, byte &value, byte min, byte max) {
	if(!config) setup();
	return pages[currentPage]->addSlider(name, value, min, max);
}

ofxSimpleGuiSliderFloat &ofxSimpleGuiToo::addSlider(string name, float &value, float min, float max, float smoothing) {
	if(!config) setup();
	return pages[currentPage]->addSlider(name, value, min, max, smoothing);
}

ofxSimpleGuiSliderDouble &ofxSimpleGuiToo::addSlider(string name, double &value, double min, double max, double smoothing) {
	if(!config) setup();
	return pages[currentPage]->addSlider(name, value, min, max, smoothing);
}

ofxSimpleGuiSlider2d &ofxSimpleGuiToo::addSlider2d(string name, ofPoint& value, float xmin, float xmax, float ymin, float ymax) {
	if(!config) setup();
	return pages[currentPage]->addSlider2d(name, value, xmin, xmax, ymin, ymax);
}

ofxSimpleGuiTitle &ofxSimpleGuiToo::addTitle(string name) {
	if(!config) setup();
	return pages[currentPage]->addTitle(name);
}

ofxSimpleGuiToggle &ofxSimpleGuiToo::addToggle(string name, bool &value) {
	if(!config) setup();
	return pages[currentPage]->addToggle(name, value);
}


//void ofxSimpleGuiToo::setup(ofEventArgs &e) {
void ofxSimpleGuiToo::update(ofEventArgs &e) {
	if(changePage) {
		nextPage();
		changePage = false;
	}

	headerPage->_update(e);
	pages[currentPage]->height = ofGetHeight();
	pages[currentPage]->_update(e);


	if(doSaveBackup) doSave = true;

	if(doSave) saveToXML(xmlFile);
}
//void ofxSimpleGuiToo::draw(ofEventArgs &e) {
//void ofxSimpleGuiToo::exit(ofEventArgs &e) {

void ofxSimpleGuiToo::mouseMoved(ofMouseEventArgs &e) {
	headerPage->_mouseMoved(e);
	pages[currentPage]->_mouseMoved(e);
}

void ofxSimpleGuiToo::mousePressed(ofMouseEventArgs &e) {
	headerPage->_mousePressed(e);
	pages[currentPage]->_mousePressed(e);
}

void ofxSimpleGuiToo::mouseDragged(ofMouseEventArgs &e) {
	headerPage->_mouseDragged(e);
	pages[currentPage]->_mouseDragged(e);
}

void ofxSimpleGuiToo::mouseReleased(ofMouseEventArgs &e) {
	headerPage->_mouseReleased(e);
	pages[currentPage]->_mouseReleased(e);
//	if(doAutoSave) doSave = true;
	if(doAutoSave) saveToXML(xmlFile);
}

void ofxSimpleGuiToo::keyPressed(ofKeyEventArgs &e) {
	headerPage->_keyPressed(e);
	pages[currentPage]->_keyPressed(e);
//	if(doAutoSave) doSave = true;
//	if(doAutoSave) saveToXML(xmlFile);
}

void ofxSimpleGuiToo::keyReleased(ofKeyEventArgs &e) {
	headerPage->_keyReleased(e);
	pages[currentPage]->_keyReleased(e);
}

/*
//------------------------------------------------------------------------ mouse moved
void ofxSimpleGuiToo::mouseMoved(int x, int y) {
	mx = x; my = y;
}

//------------------------------------------------------------------------ mouse released
void ofxSimpleGuiToo::mouseReleased() {
	if(doAutoSave) {
		saveToXML(xmlFile);
	}
}

//------------------------------------------------------------------------ key released
void ofxSimpleGuiToo::keyReleased(int key) {
	if(doAutoSave) {
		saveToXML(xmlFile);
	}
}

//------------------------------------------------------------------------ key press
void ofxSimpleGuiToo::keyPressed(int key) {

}
*/

