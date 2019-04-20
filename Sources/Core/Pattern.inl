namespace Core
{
	template<typename... Args>
	void Pattern::AddNote(float interval, Args... args)
	{ 
		_intervals.emplace_back(interval);
		_notes.emplace_back(args...);
	}
}
