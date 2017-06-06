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

#pragma once

#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/tooltip.h>
#include <wx/overlay.h>

#include <ion/core/cryptography/UUID.h>

#include <map>
#include <vector>

class KeyframePanel : public wxPanel
{
public:
	typedef ion::UUID64 SectionId;
	typedef ion::UUID64 TrackId;
	typedef ion::UUID64 KeyframeId;

	KeyframePanel(wxWindow* parent, wxWindowID windowId = wxID_ANY, const wxPoint& position = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~KeyframePanel();

	//Transport
	void Play();
	void Stop();
	void SetAtStart();
	void SetAtEnd();
	void SetPlaybackSpeed(float speed);

	void SetTime(float time);
	float GetTime() const;

	void SetLength(float length);

	//Clear all sections, tracks, and keyframes
	void Clear();

	//Add a section (group of tracks)
	SectionId AddSection(const std::string& name);
	void RemoveSection(SectionId sectionId);

	//Add a track to a section
	TrackId AddTrack(SectionId section, const std::string& name);
	void RemoveTrack(TrackId trackId);

	//Add a keyframe to a track
	KeyframeId AddKeyframe(SectionId sectionId, TrackId trackId, float time, float length, const std::string& label);
	void RemoveKeyframe(KeyframeId keyframeId);

	//Callbacks
	virtual void OnKeyframeSelected(SectionId sectionId, TrackId trackId, KeyframeId keyframeId, float time) {}
	virtual void OnKeyframeInserted(SectionId sectionId, TrackId trackId, KeyframeId keyframeId, float time) {}
	virtual void OnKeyframeDeleted(SectionId sectionId, TrackId trackId, KeyframeId keyframeId, float time) {}
	virtual void OnKeyframeMoved(SectionId sectionId, TrackId trackId, KeyframeId keyframeId, float oldTime, float newTime) {}
	virtual void OnKeyframeResized(SectionId sectionId, TrackId trackId, KeyframeId keyframeId, float oldLength, float newLength) {}

protected:
	struct Keyframe
	{
		Keyframe() {}
		Keyframe(float _time, float _length, const std::string& _label) { time = _time; length = _length; label = _label; }
		float time;
		float length;
		std::string label;
		wxRect bounds;
	};

	struct Track
	{
		Track() {}
		Track(const std::string& _name) { name = _name; }
		std::vector<std::pair<KeyframeId, Keyframe>> keyframes;
		std::string name;
	};

	struct Section
	{
		Section() {}
		Section(const std::string& _name) { name = _name; }
		std::vector<std::pair<TrackId, Track>> tracks;
		std::string name;
	};

	enum MouseOperation
	{
		eMouseOperationNone,
		eMouseOperationMoveKeyframe,
		eMouseOperationResizeKeyframe
	};

	enum ContextMenuItems
	{
		eContextMenuCopyKeyframeLeft,
		eContextMenuCopyKeyframeRight,
		eContextMenuDeleteKeyframe,
		eContextMenuUserActionStart
	};

	static const int s_timelineWidthPerSecond = 64;
	static const int s_sectionLabelDrawHeight = 20;
	static const int s_trackLabelMarginWidth = 128;
	static const int s_trackDrawHeight = 24;
	static const int s_trackDrawBorder = 2;
	static const int s_keyframeDrawMinWidth = 12;
	static const int s_keyframeDrawHeight = 24;
	static const int s_keyframeDrawBorder = 32;
	static const int s_keyframeDrawDotRadius = 4;
	static const int s_keyframeDrawTextBorder = 4;
	static const int s_keyframeResizeBorder = 8;

	Section& FindSection(SectionId sectionId);
	Track& FindTrack(SectionId sectionId, TrackId trackId);
	Keyframe& FindKeyframe(SectionId sectionId, TrackId trackId, KeyframeId keyframeId);
	bool PickKeyframe(int x, int y);

	void EventHandlerMouse(wxMouseEvent& event);
	void EventHandlerResize(wxSizeEvent& event);
	void EventHandlerPaint(wxPaintEvent& event);
	void EventHandlerContextMenu(wxCommandEvent& event);

	void Render(wxDC& dc);

	void DrawAll(wxDC& dc);
	void DrawSection(wxDC& dc, Section& section, int& offsetY);
	void DrawTrack(wxDC& dc, Track& track, int& offsetY);
	void DrawKeyframe(wxDC& dc, Keyframe& keyframe, int offsetY, bool selected);
	void DrawTransport(wxDC& dc, int offsetY);

	wxOverlay m_mainLayerSnapshot;

	std::vector<std::pair<SectionId, Section>> m_sections;

	MouseOperation m_currentMouseOperation;

	float m_animationTime;
	float m_animationLength;

	SectionId m_selectedSectionId;
	TrackId m_selectedTrackId;
	KeyframeId m_selectedKeyframeId;

	Section* m_selectedSection;
	Track* m_selectedTrack;
	Keyframe* m_selectedKeyframe;

	int m_mousePrevX;
	int m_mousePrevY;

	bool m_invalidateMainLayer;
	bool m_invalidateOverlay;
	int m_lastDrawHeight;
};