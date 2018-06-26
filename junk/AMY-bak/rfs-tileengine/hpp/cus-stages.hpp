class StageSEC  : public amy::Stage
{
	public:
		StageSEC();
		~StageSEC();
		amy::Tile* mapobj_list(int lst, int x, int y);
		void event_list(int lst, int x, int y);
};
class StageMARS : public amy::Stage
{
	public:
		StageMARS();
		~StageMARS();
		amy::Tile* mapobj_list(int lst, int x, int y);
		void event_list(int lst, int x, int y);
};
