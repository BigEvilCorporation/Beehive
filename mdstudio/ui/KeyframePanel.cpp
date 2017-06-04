///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2017 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#include "KeyframePanel.h"

#include <wx/dc.h>
#include <wx/dcclient.h>

#include <algorithm>

#include <ion/core/debug/Debug.h>

KeyframePanel::KeyframePanel(wxWindow* parent, wxWindowID windowId, const wxPoint& position, const wxSize& size, long style, const wxString& name)
	: wxPanel(parent, windowId, position, size, style, name)
{
	SetMinSize(wxSize(120, 300));

	Bind(wxEVT_SIZE, &KeyframePanel::EventHandlerResize, this, GetId());
	Bind(wxEVT_PAINT, &KeyframePanel::EventHandlerPaint, this, GetId());

	SectionId section1 = AddSection("Test Section 1");
	TrackId track1 = AddTrack(section1, "Test Track 1");
	TrackId track2 = AddTrack(section1, "Test Track 2");
	TrackId track3 = AddTrack(section1, "Test Track 3");
	AddKeyframe(section1, track1, 0.0f, 1.0f, "Test Keyframe 1");
	AddKeyframe(section1, track1, 3.0f, 6.0f, "Test Keyframe 2");
	AddKeyframe(section1, track2, 1.0f, 2.0f, "Test Keyframe 3");
	AddKeyframe(section1, track2, 3.0f, 0.1f, "Test Keyframe 4");

	SectionId section2 = AddSection("Test Section 2");
	TrackId track4 = AddTrack(section2, "Test Track 4");
	AddKeyframe(section2, track4, 7.0f, 2.0f, "Test Keyframe 5");
	AddKeyframe(section2, track4, 9.0f, 6.6f, "Test Keyframe 6");
}

KeyframePanel::~KeyframePanel()
{

}

KeyframePanel::SectionId KeyframePanel::AddSection(const std::string& name)
{
	SectionId sectionId = ion::GenerateUUID64();
	m_sections.push_back(std::make_pair(sectionId, Section(name)));
	return sectionId;
}

void KeyframePanel::RemoveSection(SectionId sectionId)
{

}

KeyframePanel::TrackId KeyframePanel::AddTrack(SectionId sectionId, const std::string& name)
{
	TrackId trackId = ion::GenerateUUID64();
	Section& section = FindSection(sectionId);
	section.tracks.push_back(std::make_pair(trackId, Track(name)));
	return trackId;
}

void KeyframePanel::RemoveTrack(TrackId trackId)
{

}

KeyframePanel::KeyframeId KeyframePanel::AddKeyframe(SectionId sectionId, TrackId trackId, float time, float length, const std::string& label)
{
	KeyframeId keyframeId = ion::GenerateUUID64();
	Track& track = FindTrack(sectionId, trackId);
	track.keyframes.push_back(std::make_pair(keyframeId, Keyframe(time, length, label)));
	return keyframeId;
}

void KeyframePanel::RemoveKeyframe(KeyframeId keyframeId)
{

}

KeyframePanel::Section& KeyframePanel::FindSection(SectionId sectionId)
{
	std::vector<std::pair<SectionId, Section>>::iterator it = std::find_if(m_sections.begin(), m_sections.end(), [&](const std::pair<SectionId, Section>& rhs) { return rhs.first == sectionId; });
	ion::debug::Assert(it != m_sections.end(), "KeyframePanel::FindSection - Invalid section");
	return it->second;
}

KeyframePanel::Track& KeyframePanel::FindTrack(SectionId sectionId, TrackId trackId)
{
	Section& section = FindSection(sectionId);
	std::vector<std::pair<TrackId, Track>>::iterator it = std::find_if(section.tracks.begin(), section.tracks.end(), [&](const std::pair<TrackId, Track>& rhs) { return rhs.first == trackId; });
	ion::debug::Assert(it != section.tracks.end(), "KeyframePanel::FindTrack - Invalid track");
	return it->second;
}

KeyframePanel::Keyframe& KeyframePanel::FindKeyframe(SectionId sectionId, TrackId trackId, KeyframeId keyframeId)
{
	Track& track = FindTrack(sectionId, trackId);
	std::vector<std::pair<KeyframeId, Keyframe>>::iterator it = std::find_if(track.keyframes.begin(), track.keyframes.end(), [&](const std::pair<KeyframeId, Keyframe>& rhs) { return rhs.first == keyframeId; });
	ion::debug::Assert(it != track.keyframes.end(), "KeyframePanel::FindKeyframe - Invalid keyframe");
	return it->second;
}

void KeyframePanel::EventHandlerResize(wxSizeEvent& event)
{
	event.Skip();
}

void KeyframePanel::EventHandlerPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	Render(dc);
}

void KeyframePanel::Render(wxDC& dc)
{
	dc.SetBrush(*wxGREY_BRUSH);
	dc.DrawRectangle(0, 0, GetSize().GetWidth(), GetSize().GetHeight());

	DrawAll(dc);
}

void KeyframePanel::DrawAll(wxDC& dc)
{
	int offsetY = 0;

	//Draw time bar
	dc.SetTextForeground(*wxWHITE);
	char textBuff[64] = { 0 };
	float time = 0.0f;
	for(int x = s_trackLabelMarginWidth; x < GetSize().GetWidth(); x += s_timelineWidthPerSecond)
	{
		sprintf_s(textBuff, 64, "%.2f", time);
		time += 1.0f;
		dc.DrawText(textBuff, x, offsetY);
	}

	offsetY += s_sectionLabelDrawHeight;

	for(int i = 0; i < m_sections.size(); i++)
	{
		const Section& section = m_sections[i].second;
		DrawSection(dc, section, offsetY);
	}
}

void KeyframePanel::DrawSection(wxDC& dc, const Section& section, int& offsetY)
{
	int height = (section.tracks.size() * (s_trackDrawHeight + s_trackDrawBorder)) + s_trackDrawBorder;

	//Draw section background
	dc.SetBrush(*wxBLUE_BRUSH);
	dc.DrawRectangle(0, offsetY, GetSize().GetWidth(), height);

	//Draw section header text
	dc.SetTextForeground(*wxWHITE);
	dc.DrawText(section.name, s_trackDrawBorder, offsetY);

	offsetY += s_sectionLabelDrawHeight;

	int sectionStartY = offsetY;

	//Draw section
	for(int i = 0; i < section.tracks.size(); i++)
	{
		const Track& track = section.tracks[i].second;
		DrawTrack(dc, track, offsetY);
	}

	//Draw margin line
	dc.SetBrush(*wxBLACK_BRUSH);
	dc.DrawLine(s_trackLabelMarginWidth, sectionStartY, s_trackLabelMarginWidth, offsetY);
}

void KeyframePanel::DrawTrack(wxDC& dc, const Track& track, int& offsetY)
{
	dc.SetBrush(*wxBLUE_BRUSH);
	dc.DrawRectangle(0, offsetY, GetSize().GetWidth(), s_trackDrawHeight);

	//Draw track time notches
	for(int x = s_trackLabelMarginWidth; x < GetSize().GetWidth(); x += s_timelineWidthPerSecond)
	{
		dc.SetBrush(*wxBLACK_BRUSH);
		dc.DrawLine(x, offsetY, x, offsetY + s_trackDrawHeight);
	}

	//Draw track label
	dc.SetTextForeground(*wxBLACK);
	dc.DrawText(track.name, s_trackDrawBorder, offsetY);

	//Draw track keyframes
	for(int i = 0; i < track.keyframes.size(); i++)
	{
		const Keyframe& keyframe = track.keyframes[i].second;
		DrawKeyframe(dc, keyframe, offsetY);
	}

	offsetY += s_trackDrawHeight;
}

void KeyframePanel::DrawKeyframe(wxDC& dc, const Keyframe& keyframe, int offsetY)
{
	int offsetX = s_trackLabelMarginWidth + ceil(keyframe.time * (float)s_timelineWidthPerSecond);
	offsetY += (s_trackDrawHeight / 2) - (s_keyframeDrawHeight / 2);
	int drawWidth = ceil(keyframe.length * (float)s_timelineWidthPerSecond);

	if(drawWidth < s_keyframeDrawMinWidth)
	{
		drawWidth = s_keyframeDrawMinWidth;
	}

	dc.SetBrush(*wxWHITE_BRUSH);
	dc.DrawRectangle(offsetX, offsetY, drawWidth, s_keyframeDrawHeight);

	//If text fits, draw it
	wxSize textSize = dc.GetTextExtent(keyframe.label);

	if(textSize.GetWidth() < (drawWidth - (s_keyframeDrawTextBorder*2)))
	{
		dc.SetTextForeground(*wxBLACK);
		dc.DrawText(keyframe.label, offsetX + s_keyframeDrawTextBorder, offsetY);
	}
	else
	{
		//Draw dot
		dc.SetBrush(*wxBLACK_BRUSH);
		dc.DrawCircle(offsetX + (drawWidth / 2), offsetY + (s_keyframeDrawHeight / 2), s_keyframeDrawDotRadius);
	}
}