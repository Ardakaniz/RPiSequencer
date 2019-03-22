namespace Core
{
	template<typename... Args>
	void Pattern::AddNote(Args... args)
	{ _notes.emplace_back(args...); }
}