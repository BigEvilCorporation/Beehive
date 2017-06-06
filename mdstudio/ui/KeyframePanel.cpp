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
#include <wx/menu.h>

#include <algorithm>

#include <ion/core/debug/Debug.h>
#include <ion/maths/Maths.h>

KeyframePanel::KeyframePanel(wxWindow* parent, wxWindowID windowId, const wxPoint& position, const wxSize& size, long style, const wxString& name)
	: wxPanel(parent, windowId, position, size, style, name)
{
	SetMinSize(wxSize(120, 300));

	m_selectedSectionId = 0;
	m_selectedTrackId = 0;
	m_selectedKeyframeId = 0;
	m_selectedKeyframe = NULL;

	m_invalidateMainLayer = true;
	m_invalidateOverlay = true;

	m_tooltip = NULL;

	m_currentMouseOperation = eMouseOperationNone;

	m_animationTime = 0.0f;
	m_animationLength = 10.0f;
	
	Bind(wxEVT_LEFT_DOWN, &KeyframePanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_LEFT_UP, &KeyframePanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_RIGHT_DOWN, &KeyframePanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_MOTION, &KeyframePanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_SIZE, &KeyframePanel::EventHandlerResize, this, GetId());
	Bind(wxEVT_PAINT, &KeyframePanel::EventHandlerPaint, this, GetId());
}

KeyframePanel::~KeyframePanel()
{

}

void KeyframePanel::SetTime(float time)
{
	m_animationTime = time;
	m_invalidateOverlay = true;
	Refresh();
}

float KeyframePanel::GetTime() const
{
	return m_animationTime;
}

void KeyframePanel::SetLength(float length)
{
	m_animationLength = length;
}

void KeyframePanel::Clear()
{
	m_sections.clear();
}

KeyframePanel::SectionId KeyframePanel::AddSection(const std::string& name)
{
	SectionId sectionId = ion::GenerateUUID64();
	m_sections.push_back(std::make_pair(sectionId, Section(name)));
	m_invalidateMainLayer = true;
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
	m_invalidateMainLayer = true;
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
	m_invalidateMainLayer = true;
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

bool KeyframePanel::PickKeyframe(int x, int y)
{
	for(int i = 0; i < m_sections.size(); i++)
	{
		Section& section = m_sections[i].second;

		for(int j = 0; j < section.tracks.size(); j++)
		{
			Track& track = section.tracks[j].second;

			for(int k = 0; k < track.keyframes.size(); k++)
			{
				Keyframe& keyframe = track.keyframes[k].second;

				if(x >= keyframe.bounds.GetLeft()
					&& x < keyframe.bounds.GetRight()
					&& y >= keyframe.bounds.GetTop()
					&& y < keyframe.bounds.GetBottom())
				{
					m_selectedSectionId = m_sections[i].first;
					m_selectedTrackId = section.tracks[j].first;
					m_selectedKeyframeId = track.keyframes[k].first;

					m_selectedSection = &section;
					m_selectedTrack = &track;
					m_selectedKeyframe = &keyframe;

					return true;
				}
			}
		}
	}

	m_selectedSectionId = 0;
	m_selectedTrackId = 0;
	m_selectedKeyframeId = 0;

	m_selectedSection = NULL;
	m_selectedTrack = NULL;
	m_selectedKeyframe = NULL;

	return false;
}

void KeyframePanel::EventHandlerMouse(wxMouseEvent& event)
{
	if(event.GetEventType() == wxEVT_LEFT_DOWN)
	{
		PickKeyframe(event.GetX(), event.GetY());
		m_invalidateMainLayer = true;
		Refresh();
	}
	if(event.GetEventType() == wxEVT_LEFT_UP)
	{
		m_currentMouseOperation = eMouseOperationNone;
	}
	if(event.GetEventType() == wxEVT_RIGHT_DOWN)
	{
		//Right-click menu
		wxMenu contextMenu;

		contextMenu.Append(eContextMenuDeleteKeyframe, wxString("Duplicate keyframe (to left)"));
		contextMenu.Append(eContextMenuDeleteKeyframe, wxString("Duplicate keyframe (to right)"));
		contextMenu.Append(eContextMenuDeleteKeyframe, wxString("Delete keyframe"));
		contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&KeyframePanel::EventHandlerContextMenu, NULL, this);

		PopupMenu(&contextMenu);
	}
	else if(event.GetEventType() == wxEVT_MOTION)
	{
		//Standard cursor by default
		wxCursor cursor(wxCURSOR_ARROW);

		int x = event.GetX();
		int y = event.GetY();
		int mouseDeltaX = m_mousePrevX - x;
		int mouseDeltaY = m_mousePrevY - y;

		if(m_selectedKeyframe)
		{
			if(x >= m_selectedKeyframe->bounds.GetLeft() && x < m_selectedKeyframe->bounds.GetRight()
				&& y >= m_selectedKeyframe->bounds.GetTop() && y < m_selectedKeyframe->bounds.GetBottom())
			{
				//If at keyframe edge, set to resize
				if((m_currentMouseOperation == eMouseOperationNone || m_currentMouseOperation == eMouseOperationResizeKeyframe)
					&& x >= (m_selectedKeyframe->bounds.GetRight() - s_keyframeResizeBorder))
				{
					//Resizing
					cursor = wxCURSOR_SIZEWE;

					if(event.Dragging())
					{
						float prevLength = m_selectedKeyframe->length;
						m_selectedKeyframe->length += (float)-mouseDeltaX / (float)s_timelineWidthPerSecond;
						m_selectedKeyframe->length = ion::maths::Clamp(m_selectedKeyframe->length, 0.1f, m_animationLength - m_selectedKeyframe->time);
						OnKeyframeResized(m_selectedSectionId, m_selectedTrackId, m_selectedKeyframeId, prevLength, m_selectedKeyframe->length);
						m_invalidateMainLayer = true;
						Refresh();

						m_currentMouseOperation = eMouseOperationResizeKeyframe;
					}
				}
				else if(m_currentMouseOperation == eMouseOperationNone || m_currentMouseOperation == eMouseOperationMoveKeyframe)
				{
					//Moving
					if(event.Dragging())
					{
						float prevTime = m_selectedKeyframe->time;
						m_selectedKeyframe->time += (float)-mouseDeltaX / (float)s_timelineWidthPerSecond;
						m_selectedKeyframe->time = ion::maths::Clamp(m_selectedKeyframe->time, 0.0f, m_animationLength - m_selectedKeyframe->length);
						OnKeyframeMoved(m_selectedSectionId, m_selectedTrackId, m_selectedKeyframeId, prevTime, m_selectedKeyframe->time);
						m_invalidateMainLayer = true;
						Refresh();

						m_currentMouseOperation = eMouseOperationMoveKeyframe;
					}
				}
			}
		}

		m_mousePrevX = event.GetX();
		m_mousePrevY = event.GetY();

		wxSetCursor(cursor);
	}

	event.Skip();
}

void KeyframePanel::EventHandlerResize(wxSizeEvent& event)
{
	event.Skip();
}

void KeyframePanel::EventHandlerPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	Render(dc);
	event.Skip();
}

void KeyframePanel::EventHandlerContextMenu(wxCommandEvent& event)
{
	switch(event.GetId())
	{
	case eContextMenuCopyKeyframeLeft:
		break;
	case eContextMenuCopyKeyframeRight:
		break;
	case eContextMenuDeleteKeyframe:
		if(m_selectedTrack && m_selectedKeyframe)
		{
			OnKeyframeDeleted(m_selectedSectionId, m_selectedTrackId, m_selectedKeyframeId, m_selectedKeyframe->time);
			m_selectedTrack->keyframes.erase(std::remove_if(m_selectedTrack->keyframes.begin(), m_selectedTrack->keyframes.end(), [&](const std::pair<KeyframeId, Keyframe>& rhs) { return rhs.first == m_selectedKeyframeId; }), m_selectedTrack->keyframes.end());
			m_selectedKeyframe = NULL;
			m_selectedKeyframeId = 0;
			m_invalidateMainLayer = true;
			Refresh();
		}
		break;
	}
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

	dc.SetPen(*wxBLACK_PEN);

	if(m_invalidateMainLayer)
	{
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

		//Draw sections
		for(int i = 0; i < m_sections.size(); i++)
		{
			Section& section = m_sections[i].second;
			DrawSection(dc, section, offsetY);
		}

		m_lastDrawHeight = offsetY;

		m_invalidateMainLayer = false;
		m_invalidateOverlay = true;

		//Take snapshot of current window
		m_mainLayerSnapshot.Reset();
	}

	if(m_invalidateOverlay)
	{
		//Restore snapshot of current window
		wxDCOverlay overlayDC(m_mainLayerSnapshot, &dc);
		overlayDC.Clear();

		//Draw transport
		DrawTransport(dc, m_lastDrawHeight);

		m_invalidateOverlay = false;
	}
}

void KeyframePanel::DrawTransport(wxDC& dc, int offsetY)
{
	//Draw current time position
	int timeX = s_trackLabelMarginWidth + (int)(m_animationTime * (float)s_timelineWidthPerSecond);
	dc.SetPen(*wxWHITE_PEN);
	dc.DrawLine(timeX, 0, timeX, offsetY);

	//Draw animation length boundary
	int lengthX = s_trackLabelMarginWidth + (int)(m_animationLength * (float)s_timelineWidthPerSecond);
	dc.SetPen(*wxRED_PEN);
	dc.DrawLine(lengthX, 0, lengthX, offsetY);
}

void KeyframePanel::DrawSection(wxDC& dc, Section& section, int& offsetY)
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
		Track& track = section.tracks[i].second;
		DrawTrack(dc, track, offsetY);
	}

	//Draw margin line
	dc.SetBrush(*wxBLACK_BRUSH);
	dc.DrawLine(s_trackLabelMarginWidth, sectionStartY, s_trackLabelMarginWidth, offsetY);
}

void KeyframePanel::DrawTrack(wxDC& dc, Track& track, int& offsetY)
{
	dc.SetBrush(*wxBLUE_BRUSH);
	dc.DrawRectangle(0, offsetY, GetSize().GetWidth(), s_trackDrawHeight);

	//Draw track time notches
	for(int x = s_trackLabelMarginWidth; x < GetSize().GetWidth(); x += s_timelineWidthPerSecond)
	{
		dc.SetBrush(*wxWHITE_BRUSH);
		dc.DrawLine(x, offsetY, x, offsetY + s_trackDrawHeight);
	}

	//Draw track label
	dc.SetTextForeground(*wxBLACK);
	dc.DrawText(track.name, s_trackDrawBorder, offsetY);

	//Draw track keyframes (in reverse)
	for(int i = track.keyframes.size() - 1; i >= 0; i--)
	{
		Keyframe& keyframe = track.keyframes[i].second;
		bool selected = (track.keyframes[i].first == m_selectedKeyframeId);
		DrawKeyframe(dc, keyframe, offsetY, selected);
	}

	offsetY += s_trackDrawHeight;
}

void KeyframePanel::DrawKeyframe(wxDC& dc, Keyframe& keyframe, int offsetY, bool selected)
{
	int offsetX = s_trackLabelMarginWidth + (keyframe.time * (float)s_timelineWidthPerSecond);
	offsetY += (s_trackDrawHeight / 2) - (s_keyframeDrawHeight / 2);
	int drawWidth = keyframe.length * (float)s_timelineWidthPerSecond;

	if(drawWidth < s_keyframeDrawMinWidth)
	{
		drawWidth = s_keyframeDrawMinWidth;
	}

	if(selected)
		dc.SetBrush(*wxYELLOW_BRUSH);
	else
		dc.SetBrush(*wxWHITE_BRUSH);

	dc.DrawRectangle(offsetX, offsetY, drawWidth, s_keyframeDrawHeight);

	//If text fits, or selected, draw it
	wxSize textSize = dc.GetTextExtent(keyframe.label);
	bool textFits = (textSize.GetWidth() < (drawWidth - (s_keyframeDrawTextBorder * 2)));

	if(selected || textFits)
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

	keyframe.bounds.SetLeft(offsetX);
	keyframe.bounds.SetTop(offsetY);
	keyframe.bounds.SetWidth(drawWidth);
	keyframe.bounds.SetHeight(s_keyframeDrawHeight);
}