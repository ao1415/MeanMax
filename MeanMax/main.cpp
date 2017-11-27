#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <array>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;

/// <summary>フィールドサイズ</summary>
const int FieldRadius = 6000;
/// <summary>水場サイズ</summary>
const int WaterRedius = 3000;

///<sumarry>加速に使われる係数</sumarry>
const double ReaperMass = 0.5;
///<sumarry>停止に使われる係数</sumarry>
const double ReaperFriction = 0.2;
///<sumarry>加速に使われる係数</sumarry>
const double DestroyerMass = 1.5;
///<sumarry>停止に使われる係数</sumarry>
const double DoofFriction = 0.25;
///<sumarry>加速に使われる係数</sumarry>
const double DoofMass = 1.0;
///<sumarry>停止に使われる係数</sumarry>
const double DestroyerFriction = 0.3;
///<sumarry>加速に使われる係数</sumarry>
const double TankerMassBase = 2.5;
///<sumarry>停止に使われる係数</sumarry>
const double TankerFriction = 0.4;

/// <summary>プレイヤーサイズ</summary>
const int PlayerRadius = 400;

const int PlayerId = 0;

const int ReaperType = 0;
const int DestroyerType = 1;
const int DoofType = 2;
const int TankerType = 3;
const int WreckType = 4;
const int TarType = 5;
const int OilType = 6;

const int ThrottleMaxPower = 300;

const int RageCapacity = 300;

const int TarCost = 30;
/// <summary>爆破範囲</summary>
const int TarRadius = 1000;
/// <summary>投擲距離</summary>
const int TarRange = 2000;
const int GrenadeCost = 60;
/// <summary>爆破範囲</summary>
const int GrenadeRadius = 1000;
/// <summary>投擲距離</summary>
const int GrenadeRange = 2000;
const int OilCost = 30;
/// <summary>爆破範囲</summary>
const int OilRadius = 1000;
/// <summary>投擲距離</summary>
const int OilRange = 2000;

const int Inf = 999999;
constexpr double pi = 3.141592653589793238;

struct Point {
	int x;
	int y;

	constexpr Point() :Point(Inf, Inf) {}
	constexpr Point(const int _x, const int _y) : x(_x), y(_y) {}

	const Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
	const Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }

	operator bool() const { return !(x == Inf && y == Inf); }

	const string toString() const { return to_string(x) + " " + to_string(y); }

};
struct Vec {
	double x;
	double y;

	constexpr Vec() :Vec(Inf, Inf) {}
	constexpr Vec(const double _x, const double _y) : x(_x), y(_y) {}

	const Vec operator+(const Vec& p) const { return Vec(x + p.x, y + p.y); }
	const Vec operator-(const Vec& p) const { return Vec(x - p.x, y - p.y); }

	operator bool() const { return !(x == Inf && y == Inf); }

	const string toString() const { return to_string(x) + " " + to_string(y); }

};

/// <summary>
/// 一定時間の経過したか確認するクラス
/// </summary>
class Timer {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Timer() = default;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(ナノ秒)</param>
	Timer(const std::chrono::nanoseconds& _time) { type = Type::nanoseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(マイクロ秒)</param>
	Timer(const std::chrono::microseconds& _time) { type = Type::microseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	Timer(const std::chrono::milliseconds& _time) { type = Type::milliseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(秒)</param>
	Timer(const std::chrono::seconds& _time) { type = Type::seconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(分)</param>
	Timer(const std::chrono::minutes& _time) { type = Type::minutes; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(時)</param>
	Timer(const std::chrono::hours& _time) { type = Type::hours; time = _time.count(); }

	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(ナノ秒)</param>
	void set(const std::chrono::nanoseconds& _time) { type = Type::nanoseconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(マイクロ秒)</param>
	void set(const std::chrono::microseconds& _time) { type = Type::microseconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	void set(const std::chrono::milliseconds& _time) { type = Type::milliseconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(秒)</param>
	void set(const std::chrono::seconds& _time) { type = Type::seconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(分</param>
	void set(const std::chrono::minutes& _time) { type = Type::minutes; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(時)</param>
	void set(const std::chrono::hours& _time) { type = Type::hours; time = _time.count(); }

	/// <summary>
	/// タイマーを開始させる
	/// </summary>
	void start() { s = std::chrono::high_resolution_clock::now(); }

	/// <summary>
	/// 設定時間経過したかを得る
	/// </summary>
	/// <returns>経過していれば true, それ以外は false</returns>
	inline const bool check() const {
		const auto e = std::chrono::high_resolution_clock::now();
		switch (type)
		{
		case Type::nanoseconds: return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() >= time;
		case Type::microseconds: return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count() >= time;
		case Type::milliseconds: return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() >= time;
		case Type::seconds: return std::chrono::duration_cast<std::chrono::seconds>(e - s).count() >= time;
		case Type::minutes: return std::chrono::duration_cast<std::chrono::minutes>(e - s).count() >= time;
		case Type::hours: return std::chrono::duration_cast<std::chrono::hours>(e - s).count() >= time;
		default: return true;
		}
	}

	/// <summary>
	/// 設定時間経過したかを得る
	/// </summary>
	/// <returns>経過していれば true, それ以外は false</returns>
	operator bool() const { return check(); }

private:

	enum class Type {
		nanoseconds,
		microseconds,
		milliseconds,
		seconds,
		minutes,
		hours
	};

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	long long time;
	Type type;

};

/// <summary>
/// 時間計測を行うクラス
/// </summary>
class Stopwatch {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stopwatch() = default;

	/// <summary>
	/// 計測を開始する
	/// </summary>
	inline void start() {
		s = std::chrono::high_resolution_clock::now();
		e = s;
	}
	/// <summary>
	/// 計測を停止する
	/// </summary>
	inline void stop() {
		e = std::chrono::high_resolution_clock::now();
	}

	/// <summary>
	/// 計測時間を取得する(ナノ秒)
	/// </summary>
	/// <returns>計測時間(ナノ秒)</returns>
	inline const long long nanoseconds() const { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
	/// <summary>
	/// 計測時間を取得する(マイクロ秒)
	/// </summary>
	/// <returns>計測時間(マイクロ秒)</returns>
	inline const long long microseconds() const { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
	/// <summary>
	/// 計測時間を取得する(ミリ秒)
	/// </summary>
	/// <returns>計測時間(ミリ秒)</returns>
	inline const long long millisecond() const { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }
	/// <summary>
	/// 計測時間を取得する(秒)
	/// </summary>
	/// <returns>計測時間(秒)</returns>
	inline const long long second() const { return std::chrono::duration_cast<std::chrono::seconds>(e - s).count(); }
	/// <summary>
	/// 計測時間を取得する(分)
	/// </summary>
	/// <returns>計測時間(分)</returns>
	inline const long long minutes() const { return std::chrono::duration_cast<std::chrono::minutes>(e - s).count(); }
	/// <summary>
	/// 計測時間を取得する(時)
	/// </summary>
	/// <returns>計測時間(時)</returns>
	inline const long long hours() const { return std::chrono::duration_cast<std::chrono::hours>(e - s).count(); }

	/// <summary>
	/// 単位付きの計測時間の文字列を得る(ナノ秒)
	/// </summary>
	/// <returns>計測時間の文字列(ナノ秒)</returns>
	inline const std::string toString_ns() const { return std::to_string(nanoseconds()) + "ns"; }
	/// <summary>
	/// 単位付きの計測時間の文字列を得る(マイクロ秒)
	/// </summary>
	/// <returns>計測時間の文字列(マイクロ秒)</returns>
	inline const std::string toString_us() const { return std::to_string(microseconds()) + "us"; }
	/// <summary>
	/// 単位付きの計測時間の文字列を得る(ミリ秒)
	/// </summary>
	/// <returns>計測時間の文字列(ミリ秒)</returns>
	inline const std::string toString_ms() const { return std::to_string(millisecond()) + "ms"; }
	/// <summary>
	/// 単位付きの計測時間の文字列を得る(秒)
	/// </summary>
	/// <returns>計測時間の文字列(秒)</returns>
	inline const std::string toString_s() const { return std::to_string(second()) + "s"; }
	/// <summary>
	/// 単位付きの計測時間の文字列を得る(分)
	/// </summary>
	/// <returns>計測時間の文字列(分)</returns>
	inline const std::string toString_m() const { return std::to_string(minutes()) + "m"; }
	/// <summary>
	/// 単位付きの計測時間の文字列を得る(時)
	/// </summary>
	/// <returns>計測時間の文字列(時)</returns>
	inline const std::string toString_h() const { return std::to_string(hours()) + "h"; }

private:

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	std::chrono::time_point<std::chrono::high_resolution_clock> e;

};

template<typename Type, size_t Width, size_t Height>
class FixedGrid {
private:

	using ContainerType = std::array<Type, Width * Height>;
	ContainerType m_data;

public:

	FixedGrid() = default;
	FixedGrid(const Type& v) { fill(v); }
	FixedGrid(const FixedGrid& other) = default;
	FixedGrid(FixedGrid&& other) {
		m_data = std::move(other.m_data);
	}

	FixedGrid& operator=(const FixedGrid& other) = default;
	FixedGrid& operator=(FixedGrid&& other) = default;

	const Type* operator[](size_t y) const {
		return &m_data[y * Width];
	}
	Type* operator[](size_t y) {
		return &m_data[y * Width];
	}

	const Type& at(size_t x, size_t y) const {
		if (outside(x, y))
			throw std::out_of_range("FixedGrid::at");
		return m_data[y * Width + x];
	}
	Type& at(size_t x, size_t y) {
		if (outside(x, y))
			throw std::out_of_range("FixedGrid::at");
		return m_data[y * Width + x];
	}

	constexpr int width() const {
		return static_cast<int>(Width);
	}
	constexpr int height() const {
		return static_cast<int>(Height);
	}

	bool inside(size_t x, size_t y) const {
		return (0 <= x && x < Width && 0 <= y && y < Height);
	}
	bool outside(size_t x, size_t y) const {
		return (0 > x || x >= Width || 0 > y || y >= Height);
	}

	void fill(const Type& v) noexcept {
		m_data.fill(v);
	}

	void clear() {
		m_data.swap(ContainerType());
	}

};

struct Entitie {

	int id;
	/// <summary>
	/// <para>0 : Reaper</para>
	/// <para>1 : Destroyer</para>
	/// <para>2 : Doof</para>
	/// <para>3 : Tanker</para>
	/// <para>4 : Wreck</para>
	/// <para>5 : Tar</para>
	/// <para>6 : Oil</para>
	/// </summary>
	int type;
	/// <summary>
	/// プレイヤーのID
	/// </summary>
	int player;
	double mass;
	int radius;
	Point pos;
	Point v;
	/// <summary>
	/// <para>タンカーの水量</para>
	/// <para>スキルの残り時間</para>
	/// </summary>
	int extra1;
	/// <summary>
	/// タンカーのキャパ
	/// </summary>
	int extra2;

};

inline std::ostream& operator << (ostream& os, const Point& p) { return os << "(" << p.x << "," << p.y << ")"; }
inline std::ostream& operator << (ostream& os, const Vec& v) { return os << "(" << v.x << "," << v.y << ")"; }

struct Input;

using EnemyReapers = vector<Entitie>;
using EnemyDestroyers = vector<Entitie>;
using EnemyDoofs = vector<Entitie>;
using PlayerReapers = Entitie;
using PlayerDestroyers = Entitie;
using PlayerDoofs = Entitie;
using Tankers = vector<Entitie>;
using Wreckses = vector<Entitie>;
using Tars = vector<Entitie>;
using Oils = vector<Entitie>;

class Share {
public:

	friend Input;

	inline static const EnemyReapers& getEnReapers() {
		return enReapers;
	}
	inline static const EnemyDestroyers& getEnDestroyers() {
		return enDestroyers;
	}
	inline static const EnemyDoofs& getEnDoofs() {
		return enDoofs;
	}

	inline static const PlayerReapers& getMyReapers() {
		return myReapers;
	}
	inline static const PlayerDestroyers& getMyDestroyers() {
		return myDestroyers;
	}
	inline static const PlayerDoofs& getMyDoofs() {
		return myDoofs;
	}

	inline static const Tankers& getTankers() {
		return tankers;
	}

	inline static const Wreckses& getWrecks() {
		return wrecks;
	}

	inline static const Tankers& getfTankers() {
		return ftankers;
	}

	inline static const Wreckses& getfWrecks() {
		return fwrecks;
	}

	inline static const Tars& getTars() {
		return tars;
	}
	inline static const Tars& getOils() {
		return oils;
	}

	inline static const int& getMyRage() {
		return myRage;
	}

	inline static void useRage(const int rage) {
		myRage -= rage;
	}

	inline static int getTopId() {
		return enScore1 > enScore2 ? 1 : 2;
	}

	inline static void add_wreck(const Entitie& e) {
		fwrecks.push_back(e);
	}

private:

	static EnemyReapers enReapers;
	static EnemyDestroyers enDestroyers;
	static EnemyDoofs enDoofs;
	static PlayerReapers myReapers;
	static PlayerDestroyers myDestroyers;
	static PlayerDoofs myDoofs;
	static Tankers tankers;
	static Tankers ftankers;
	static Wreckses wrecks;
	static Wreckses fwrecks;
	static Tars tars;
	static Oils oils;

	static int myRage;
	static int enRage1;
	static int enRage2;

	static int myScore;
	static int enScore1;
	static int enScore2;

};

EnemyReapers Share::enReapers;
EnemyDestroyers Share::enDestroyers;
EnemyDoofs Share::enDoofs;
PlayerReapers Share::myReapers;
PlayerDestroyers Share::myDestroyers;
PlayerDoofs Share::myDoofs;
Tankers Share::tankers;
Tankers Share::ftankers;
Wreckses Share::wrecks;
Wreckses Share::fwrecks;
Tars Share::tars;
Oils Share::oils;

int Share::myRage;
int Share::enRage1;
int Share::enRage2;

int Share::myScore;
int Share::enScore1;
int Share::enScore2;

struct Input {

	static void first() {

	}

	static void loop() {

		Share::enReapers.clear();
		Share::enDestroyers.clear();
		Share::enDoofs.clear();
		Share::tankers.clear();
		Share::wrecks.clear();
		Share::tankers.clear();
		Share::oils.clear();

		cin >> Share::myScore; cin.ignore();
		cin >> Share::enScore1; cin.ignore();
		cin >> Share::enScore2; cin.ignore();

		cin >> Share::myRage; cin.ignore();
		cin >> Share::enRage1; cin.ignore();
		cin >> Share::enRage2; cin.ignore();

		int unitCount;
		cin >> unitCount; cin.ignore();
		for (int i = 0; i < unitCount; i++) {
			Entitie e;
			cin >> e.id >> e.type >> e.player >> e.mass >> e.radius >> e.pos.x >> e.pos.y >> e.v.x >> e.v.y >> e.extra1 >> e.extra2;
			cin.ignore();

			switch (e.type)
			{
			case ReaperType:
				if (e.player == PlayerId)
					Share::myReapers = std::move(e);
				else
					Share::enReapers.emplace_back(std::move(e));
				break;

			case DestroyerType:
				if (e.player == PlayerId)
					Share::myDestroyers = std::move(e);
				else
					Share::enDestroyers.emplace_back(std::move(e));
				break;

			case DoofType:
				if (e.player == PlayerId)
					Share::myDoofs = std::move(e);
				else
					Share::enDoofs.emplace_back(std::move(e));
				break;

			case TankerType:
				Share::tankers.emplace_back(std::move(e));
				break;

			case WreckType:
				Share::wrecks.emplace_back(std::move(e));
				break;

			case TarType:
				Share::tars.emplace_back(std::move(e));
				break;

			case OilType:
				Share::oils.emplace_back(std::move(e));
				break;

			default:
				break;
			}
		}
		Share::fwrecks = Share::wrecks;
		Share::ftankers = Share::tankers;

	}

};

const string throttle(const Point& pos, const int power, const string& mes = "-") {
	stringstream ss;
	ss << pos.x << " " << pos.y << " " << power << " " << mes;
	return ss.str();
}

const string skill(const Point& pos, const string& mes = "-") {
	stringstream ss;
	ss << "SKILL " << pos.x << " " << pos.y << " " << mes;
	return ss.str();
}

const string wait(const string& mes = "-") {
	stringstream ss;
	ss << "WAIT " << " " << mes;
	return ss.str();
}

const int range(const Point& p1, const Point& p2) {
	const int dx = p2.x - p1.x;
	const int dy = p2.y - p1.y;
	return dx*dx + dy*dy;
}
constexpr int range(int r) {
	return r*r;
}

const string speedControl(const Point& start, const Point& end, const Point& vec, const int type) {

	const double mass = [&]() {
		switch (type)
		{
		case ReaperType: return ReaperMass;
		case DestroyerType: return DestroyerMass;
		case DoofType: return DoofMass;
		}
		return 1.0;
	}();
	const double friction = [&]() {
		switch (type)
		{
		case ReaperType: return ReaperFriction;
		case DestroyerType: return DestroyerFriction;
		case DoofType: return DoofFriction;
		}
		return 1.0;
	}();

	[&](const Point& v, const double& angle, const int throttle) {
		const double dx = (v.x + (cos(angle)*throttle / mass))*(1 - friction);
		const double dy = (v.y + (sin(angle)*throttle / mass))*(1 - friction);
	};

	//目的地までの最短ベクトル
	const auto dp = end - start;

	//最短ベクトルと現在のベクトルの差分=必要な補正量
	const auto dx = dp.x - vec.x;
	const auto dy = dp.y - vec.y;

	const int power = static_cast<int>(sqrt(dx*dx + dy*dy));

	return throttle(start + Point(dx, dy), min(power, ThrottleMaxPower));
}

constexpr double radius(int d) {
	return d * pi / 180.0;
}

class ReaperSimulation {
public:

	const string think(const Point& end) {

		const auto& reaper = Share::getMyReapers();
		Data now;
		now.pos = reaper.pos;
		now.v = reaper.v;

		const vector<Entitie>& looters = []() {

			vector<Entitie> units;

			const auto& mDestroyer = Share::getMyDestroyers();
			const auto& mDoof = Share::getMyDoofs();
			const auto& eReapers = Share::getEnReapers();
			const auto& eDestroyers = Share::getEnDestroyers();
			const auto& eDoofs = Share::getEnDoofs();
			const auto& tankers = Share::getTankers();

			units.push_back(mDestroyer);
			units.push_back(mDoof);
			for (const auto& u : eReapers) units.push_back(u);
			for (const auto& u : eDestroyers) units.push_back(u);
			for (const auto& u : eDoofs) units.push_back(u);
			for (const auto& u : tankers) units.push_back(u);

			return units;
		}();
		const vector<Entitie>& tarPools = Share::getTars();
		const vector<Entitie>& oilPools = Share::getOils();
		const vector<Entitie>& wrecks = Share::getWrecks();

		const Entities nowEntities{ looters,tarPools,oilPools,wrecks };

		const auto& field = simulation(nowEntities);

		array<priority_queue<Data>, Turn + 1> qData;
		qData[0].push(now);

		Timer timer(5ms);
		timer.start();

		while (!timer)
		{
			for (int turn = 0; turn < Turn; turn++)
			{
				for (int w = 0; w < Width; w++)
				{
					if (qData[turn].empty())
						break;

					{
						auto next = qData[turn].top();

						const auto dp = end - next.pos;
						const auto dx = dp.x - next.v.x;
						const auto dy = dp.y - next.v.y;
						const int power = static_cast<int>(sqrt(dx*dx + dy*dy));
						const auto d = Vec(dx, dy);

						if (turn == 0)
						{
							const Point distance(next.pos.x + static_cast<int>(d.x), next.pos.y + static_cast<int>(d.y));
							next.command = throttle(distance, min(power, ThrottleMaxPower));
						}

						eval(next, d, field[turn], end);

						qData[turn + 1].emplace(std::move(next));
					}

					for (const auto& d : direction)
					{
						auto next = qData[turn].top();

						if (turn == 0)
						{
							const Point distance(next.pos.x + static_cast<int>(d.x), next.pos.y + static_cast<int>(d.y));
							next.command = throttle(distance, ThrottleMaxPower);
						}

						eval(next, d, field[turn], end);

						qData[turn + 1].emplace(std::move(next));
					}
					qData[turn].pop();
				}
			}
		}

		if (!qData[Turn].empty())
		{
			return qData[Turn].top().command;
		}

		return throttle(Point(0, 0), ThrottleMaxPower, "errer");
	}

private:

	struct Data {
		string command;

		Point pos;
		Point v;

		int score = 0;

		bool operator<(const Data& other) const {
			return this->score < other.score;
		}

	};

	struct Entities {
		vector<Entitie> looters;
		vector<Entitie> tarPools;
		vector<Entitie> oilPools;
		vector<Entitie> wrecks;
	};

	const static int Turn = 5;
	const static int Width = 1;

	const static int space = 30;

	const array<Vec, 12> direction{
		Vec(cos(radius(0)) * ThrottleMaxPower,sin(radius(0)) * ThrottleMaxPower),
		Vec(cos(radius(30)) * ThrottleMaxPower,sin(radius(30)) * ThrottleMaxPower),
		Vec(cos(radius(60)) * ThrottleMaxPower,sin(radius(60)) * ThrottleMaxPower),
		Vec(cos(radius(90)) * ThrottleMaxPower,sin(radius(90)) * ThrottleMaxPower),
		Vec(cos(radius(120)) * ThrottleMaxPower,sin(radius(120)) * ThrottleMaxPower),
		Vec(cos(radius(150)) * ThrottleMaxPower,sin(radius(150)) * ThrottleMaxPower),
		Vec(cos(radius(180)) * ThrottleMaxPower,sin(radius(180)) * ThrottleMaxPower),
		Vec(cos(radius(210)) * ThrottleMaxPower,sin(radius(210)) * ThrottleMaxPower),
		Vec(cos(radius(240)) * ThrottleMaxPower,sin(radius(240)) * ThrottleMaxPower),
		Vec(cos(radius(270)) * ThrottleMaxPower,sin(radius(270)) * ThrottleMaxPower),
		Vec(cos(radius(300)) * ThrottleMaxPower,sin(radius(300)) * ThrottleMaxPower),
		Vec(cos(radius(330)) * ThrottleMaxPower,sin(radius(330)) * ThrottleMaxPower),
	};

	const array<Entities, Turn> simulation(const Entities& entities) {
		array<Entities, Turn> field;

		field[0] = entities;

		for (auto& e : field[0].looters) e.pos = e.pos + e.v;
		for (auto& p : field[0].tarPools) p.extra1--;
		for (auto& p : field[0].oilPools) p.extra1--;

		for (int turn = 1; turn < Turn; turn++)
		{

			field[turn] = field[turn - 1];

			for (auto& e : field[turn].looters) e.pos = e.pos + e.v;
			for (auto& p : field[turn].tarPools) p.extra1--;
			for (auto& p : field[turn].oilPools) p.extra1--;
		}

		return field;
	}

	void eval(Data& u, const Vec& a, const Entities& e, const Point& end) const {

		double mass = ReaperMass;

		for (const auto& p : e.tarPools)
		{
			if (p.extra1 >= 0)
			{
				if (range(p.pos, u.pos) <= range(TarRadius + PlayerRadius))
				{
					mass += 10;
				}
			}
		}

		const bool oil = [&]() {
			for (const auto& p : e.oilPools)
			{
				if (p.extra1 >= 0)
				{
					if (range(p.pos, u.pos) <= range(OilRadius + PlayerRadius))
					{
						return true;
					}
				}
			}
			return false;
		}();

		u.v.x += static_cast<int>(a.x / mass);
		u.v.y += static_cast<int>(a.y / mass);

		u.pos = u.pos + u.v;

		const int hit = [&]() {

			int c = 0;
			for (const auto& l : e.looters)
			{
				const int radius = PlayerRadius + l.radius;
				if (range(u.pos, l.pos) <= radius*radius)
				{
					c++;
				}
			}

			if (range(Point(0, 0), u.pos) >= range(FieldRadius - PlayerRadius))
				c++;

			return c;
		}();

		if (hit == 0)
		{
			if (!oil)
			{
				u.v.x = static_cast<int>(u.v.x * (1 - ReaperFriction));
				u.v.y = static_cast<int>(u.v.y * (1 - ReaperFriction));
			}

		}
		else
		{
			u.pos.x = u.pos.x - u.v.x / 2;
			u.pos.y = u.pos.y - u.v.y / 2;
			u.v.x = 0;
			u.v.y = 0;
		}

		int water = 0;
		for (const auto& w : e.wrecks)
		{
			if (range(u.pos, w.pos) < w.radius*w.radius)
			{
				water++;
			}
		}

		int score = 0;

		const int r = range(u.pos, end);

		if (r < range(400))
			score = 1000;
		else
			score -= r;

		score += water * 50;

		//cerr << "hit:" << hit << ", " << "rage:" << rage << endl;

		u.score += score;
	}
};

class DoofSimulation {
public:

	const string think() {

		const auto& doof = Share::getMyDoofs();
		Data now;
		now.pos = doof.pos;
		now.v = doof.v;
		now.rage = Share::getMyRage();

		const vector<Entitie>& looters = []() {

			vector<Entitie> units;

			const auto& mReaper = Share::getMyReapers();
			const auto& mDestroyer = Share::getMyDestroyers();
			const auto& eReapers = Share::getEnReapers();
			const auto& eDestroyers = Share::getEnDestroyers();
			const auto& eDoofs = Share::getEnDoofs();
			const auto& tankers = Share::getTankers();

			units.push_back(mReaper);
			units.push_back(mDestroyer);
			for (const auto& u : eReapers) units.push_back(u);
			for (const auto& u : eDestroyers) units.push_back(u);
			for (const auto& u : eDoofs) units.push_back(u);
			for (const auto& u : tankers) units.push_back(u);

			return units;
		}();
		const vector<Entitie>& tarPools = Share::getTars();
		const vector<Entitie>& oilPools = Share::getOils();

		const Entities nowEntities{ looters,tarPools,oilPools };

		const auto& field = simulation(nowEntities);

		array<priority_queue<Data>, Turn + 1> qData;
		qData[0].push(now);

		Timer timer(5ms);
		timer.start();

		while (!timer)
		{
			for (int turn = 0; turn < Turn; turn++)
			{
				for (int w = 0; w < Width; w++)
				{
					if (qData[turn].empty())
						break;

					for (const auto& d : direction)
					{
						auto next = qData[turn].top();

						if (turn == 0)
						{
							const Point distance(next.pos.x + static_cast<int>(d.x), next.pos.y + static_cast<int>(d.y));
							next.command = throttle(distance, ThrottleMaxPower);
						}

						eval(next, d, field[turn]);

						qData[turn + 1].emplace(std::move(next));
					}
					qData[turn].pop();
				}
			}
		}

		if (!qData[Turn].empty())
		{
			return qData[Turn].top().command;
		}

		return throttle(Point(0, 0), ThrottleMaxPower, "errer");
	}

private:

	struct Data {
		string command;

		Point pos;
		Point v;
		int rage = 0;

		int score = 0;

		bool operator<(const Data& other) const {
			return this->score < other.score;
		}

	};

	struct Entities {
		vector<Entitie> looters;
		vector<Entitie> tarPools;
		vector<Entitie> oilPools;
	};

	const static int Turn = 5;
	const static int Width = 1;

	const static int space = 30;

	const array<Vec, 12> direction{
		Vec(cos(radius(0)) * ThrottleMaxPower,sin(radius(0)) * ThrottleMaxPower),
		Vec(cos(radius(30)) * ThrottleMaxPower,sin(radius(30)) * ThrottleMaxPower),
		Vec(cos(radius(60)) * ThrottleMaxPower,sin(radius(60)) * ThrottleMaxPower),
		Vec(cos(radius(90)) * ThrottleMaxPower,sin(radius(90)) * ThrottleMaxPower),
		Vec(cos(radius(120)) * ThrottleMaxPower,sin(radius(120)) * ThrottleMaxPower),
		Vec(cos(radius(150)) * ThrottleMaxPower,sin(radius(150)) * ThrottleMaxPower),
		Vec(cos(radius(180)) * ThrottleMaxPower,sin(radius(180)) * ThrottleMaxPower),
		Vec(cos(radius(210)) * ThrottleMaxPower,sin(radius(210)) * ThrottleMaxPower),
		Vec(cos(radius(240)) * ThrottleMaxPower,sin(radius(240)) * ThrottleMaxPower),
		Vec(cos(radius(270)) * ThrottleMaxPower,sin(radius(270)) * ThrottleMaxPower),
		Vec(cos(radius(300)) * ThrottleMaxPower,sin(radius(300)) * ThrottleMaxPower),
		Vec(cos(radius(330)) * ThrottleMaxPower,sin(radius(330)) * ThrottleMaxPower),
	};

	const array<Entities, Turn> simulation(const Entities& entities) {
		array<Entities, Turn> field;

		field[0] = entities;

		for (auto& e : field[0].looters) e.pos = e.pos + e.v;
		for (auto& p : field[0].tarPools) p.extra1--;
		for (auto& p : field[0].oilPools) p.extra1--;

		for (int turn = 1; turn < Turn; turn++)
		{

			field[turn] = field[turn - 1];

			for (auto& e : field[turn].looters) e.pos = e.pos + e.v;
			for (auto& p : field[turn].tarPools) p.extra1--;
			for (auto& p : field[turn].oilPools) p.extra1--;
		}

		return field;
	}

	void eval(Data& u, const Vec& a, const Entities& e) const {

		double mass = DoofMass;
		int rage = 0;
		for (const auto& p : e.tarPools)
		{
			if (p.extra1 >= 0)
			{
				if (range(p.pos, u.pos) <= range(TarRadius + PlayerRadius))
				{
					mass += 10;
				}
			}
		}

		u.v.x += static_cast<int>(a.x / mass);
		u.v.y += static_cast<int>(a.y / mass);

		u.pos = u.pos + u.v;

		const int hit = [&]() {

			int c = 0;
			for (const auto& l : e.looters)
			{
				const int radius = PlayerRadius + l.radius;
				if (range(u.pos, l.pos) <= radius*radius)
				{
					c++;
				}
			}

			if (range(Point(0, 0), u.pos) >= range(FieldRadius - PlayerRadius))
				c++;

			return c;
		}();

		if (hit == 0)
		{
			const bool oil = [&]() {
				for (const auto& p : e.oilPools)
				{
					if (p.extra1 >= 0)
					{
						if (range(p.pos, u.pos) <= range(OilRadius + PlayerRadius))
						{
							return true;
						}
					}
				}
				return false;
			}();

			if (!oil)
			{
				u.v.x = static_cast<int>(u.v.x * (1 - DoofFriction));
				u.v.y = static_cast<int>(u.v.y * (1 - DoofFriction));
			}

			rage = static_cast<int>(sqrt(u.v.x*u.v.x + u.v.y*u.v.y));
			u.rage += rage / 100;
		}
		else
		{
			u.v.x = 0;
			u.v.y = 0;
		}

		int score = 0;

		score -= hit * 200;
		score += rage;
		score -= range(Point(0, 0), u.pos) / (1000 * 1000) * 10;

		//cerr << "hit:" << hit << ", " << "rage:" << rage << endl;

		u.score += score;
	}

};

class ScoreMap {
public:

	ScoreMap() : insideMap(makeInsideMap()) {

	}

	void makeWreckMap() {

		wreckMap_32.fill(0);
		wreckMap_25.fill(0);

		const auto& wrecks = Share::getWrecks();

		for (int y = 0; y < wreckMap_32.height(); y++)
		{
			for (int x = 0; x < wreckMap_32.width(); x++)
			{
				if (insideMap[y][x] != 0)
				{
					const Point pos(toGridPoint(x, wreckMap_32.width()), toGridPoint(y, wreckMap_32.width()));
					for (const auto& w : wrecks)
					{
						if (range(pos, w.pos) < w.radius*w.radius)
						{
							wreckMap_32[y][x] += w.extra1 == 1 ? 1 : w.extra1 / 2;
						}
					}
				}
			}
		}
		for (int y = 0; y < wreckMap_25.height(); y++)
		{
			for (int x = 0; x < wreckMap_25.width(); x++)
			{
				for (int dy = 0; dy < 8; dy++)
				{
					for (int dx = 0; dx < 8; dx++)
					{
						wreckMap_25[y][x] += wreckMap_32[y + dy][x + dx];
					}
				}
			}
		}

	}

	const Point getWrecksPosition() const {

		pair<int, Point> max(0, Point(0, 0));
		int space = FieldRadius * 2 / wreckMap_25.height();

		const int baseSpace = FieldRadius * 2 / wreckMap_32.height();
		const int basePos = static_cast<int>(baseSpace * 4.5);

		for (int y = 0; y < wreckMap_25.height(); y++)
		{
			for (int x = 0; x < wreckMap_25.width(); x++)
			{
				if (max.first > wreckMap_25[y][x])
				{
					max.first = wreckMap_25[y][x];
					max.second = Point(basePos + x*baseSpace, basePos + y*baseSpace);
				}
			}
		}

		return max.second;
	}

private:

	const FixedGrid<int, 32, 32> insideMap;

	FixedGrid<int, 32, 32> wreckMap_32;
	FixedGrid<int, 25, 25> wreckMap_25;

	const int toGridPoint(int p, int s) const {
		const int space = FieldRadius * 2 / s;
		return p*space - FieldRadius + space / 2;
	}

	const FixedGrid<int, 32, 32> makeInsideMap() {
		FixedGrid<int, 32, 32> insideMap(0);

		const int space = PlayerRadius;

		for (int y = 0; y < insideMap.height(); y++)
		{
			for (int x = 0; x < insideMap.width(); x++)
			{
				const int dx = toGridPoint(x, insideMap.width());
				const int dy = toGridPoint(y, insideMap.width());

				if (dx*dx + dy*dy <= range(FieldRadius))
					insideMap[y][x] = 1;
			}
		}

		return insideMap;
	}


};

class ReaperPos {
public:

	const Point getPos() {

		Stopwatch sw;

		const auto& my = Share::getMyReapers();
		const auto& enemy = Share::getEnReapers();

		sw.start();
		const auto& wrecks = getWrecks(Share::getWrecks());
		sw.stop();
		cerr << "Wrecks:" << sw.toString_us();

		sw.start();
		const auto& tankers = getTankers(Share::getfTankers());
		sw.stop();
		cerr << ", Tankers:" << sw.toString_us() << endl;

		sw.start();
		const auto& arrivalTurn = getArrivalTurn(my, enemy, wrecks);
		sw.stop();
		cerr << "Arrival:" << sw.toString_us() << endl;

		sw.start();
		const Point pos = eval(my, enemy, wrecks, arrivalTurn, tankers);
		sw.stop();
		cerr << "Eval:" << sw.toString_us() << endl;

		return pos;
	}

private:

	const Wreckses getWrecks(const Wreckses wrecks) const {
		Wreckses wreckPos;
		const int size = static_cast<int>(wrecks.size());

		const auto pow = [](const int r) { return r*r; };

		for (int i = 0; i < size; i++)
		{
			for (int j = i + 1; j < size; j++)
			{
				const double r = sqrt(range(wrecks[i].pos, wrecks[j].pos));
				const int radius = wrecks[i].radius + wrecks[j].radius;

				if (r < radius && ((radius - r) / 2) > 100)
				{
					const double ratio1 = wrecks[i].radius / static_cast<double>(radius);
					const double ratio2 = 1 - ratio1;

					const int dx = static_cast<int>(wrecks[i].pos.x*ratio2 + wrecks[j].pos.x*ratio1);
					const int dy = static_cast<int>(wrecks[i].pos.y*ratio2 + wrecks[j].pos.y*ratio1);

					Entitie e;
					e.pos.x = dx;
					e.pos.y = dy;
					e.extra1 = wrecks[i].extra1 + wrecks[j].extra1;
					e.radius = static_cast<int>((radius - r) / 2);
					wreckPos.emplace_back(std::move(e));
				}
			}
			wreckPos.push_back(wrecks[i]);
		}

		return wreckPos;
	}

	const array<vector<int>, 3> getArrivalTurn(const Entitie& my, const EnemyReapers& enemy, const Wreckses& wrecks) const {
		array<vector<int>, 3> arrivalTurn;

		const auto getTurn = [](Point pos, Point v, const Point& end, const int R) {

			const double mass = ReaperMass;
			const double friction = ReaperFriction;

			int turn = 0;

			while (range(pos, end) > R*R && turn < 20)
			{
				turn++;

				const auto dp = end - pos;
				const auto dx = dp.x - v.x;
				const auto dy = dp.y - v.y;
				const int r = static_cast<int>(sqrt(dx*dx + dy*dy));
				const int power = min(r, ThrottleMaxPower);
				const double ratio = (static_cast<double>(power) / mass) / r;

				v.x += static_cast<int>(dx*ratio);
				v.y += static_cast<int>(dy*ratio);
				pos = pos + v;
				v.x = static_cast<int>(v.x*(1 - friction));
				v.y = static_cast<int>(v.y*(1 - friction));
			}

			return turn;
		};

		for (const auto& w : wrecks)
		{
			cerr << "my:" << my.pos << ", " << my.v << ", " << w.pos << ", " << w.radius << endl;
			const int turn1 = getTurn(my.pos, my.v, w.pos, w.radius);
			arrivalTurn[PlayerId].push_back(turn1);

			cerr << "e1:" << enemy[0].pos << ", " << enemy[0].v << ", " << w.pos << ", " << w.radius << endl;
			const int turn2 = getTurn(enemy[0].pos, enemy[0].v, w.pos, w.radius);
			arrivalTurn[enemy[0].player].push_back(turn2);

			cerr << "e2:" << enemy[1].pos << ", " << enemy[1].v << ", " << w.pos << ", " << w.radius << endl;
			const int turn3 = getTurn(enemy[1].pos, enemy[1].v, w.pos, w.radius);
			arrivalTurn[enemy[1].player].push_back(turn3);
		}

		return arrivalTurn;
	}

	const array<Tankers, 10> getTankers(const Tankers& tankers) const {
		array<Tankers, 10> tankersSimu;

		tankersSimu[0] = tankers;

		for (int t = 1; t < (int)tankersSimu.size(); t++)
		{
			tankersSimu[t] = tankersSimu[t - 1];

			for (auto& t : tankersSimu[t])
			{
				const double angel = atan2(t.pos.y, t.pos.x);
				const double mass = TankerMassBase + 0.5*t.extra1;
				const double friction = TankerFriction;
				if (t.extra1 < t.extra2)
				{
					if (range(Point(0, 0), t.pos) > range(WaterRedius))
					{
						t.v.x += static_cast<int>(cos(angel + pi) * 500 / mass);
						t.v.y += static_cast<int>(sin(angel + pi) * 500 / mass);
					}
					else
					{
						t.extra1++;
					}
					t.pos = t.pos + t.v;
					t.v.x = static_cast<int>(t.v.x*(1 - friction));
					t.v.y = static_cast<int>(t.v.y*(1 - friction));
				}
				else
				{
					t.v.x += static_cast<int>(cos(angel) * 500 / mass);
					t.v.y += static_cast<int>(sin(angel) * 500 / mass);

					t.pos = t.pos + t.v;
					t.v.x = static_cast<int>(t.v.x*(1 - friction));
					t.v.y = static_cast<int>(t.v.y*(1 - friction));
				}
			}
		}

		return tankersSimu;
	}

	const Point eval(const Entitie& my, const EnemyReapers& enemy, const Wreckses& wrecks, const array<vector<int>, 3>& arrivalTurn, const array<Tankers, 10>& tankers) const {

		int minScore = 0;
		Point pos;

		const int size = static_cast<int>(wrecks.size());
		for (int i = 0; i < size; i++)
		{
			array<int, 3> arrival;
			arrival[0] = arrivalTurn[0][i];
			arrival[1] = arrivalTurn[1][i];
			arrival[2] = arrivalTurn[2][i];

			const int score = eval(my, enemy, wrecks[i], arrival, tankers);
			cerr << score << ", " << arrival[0] << endl;

			if (minScore < score)
			{
				minScore = score;
				pos = wrecks[i].pos;
			}

		}

		return pos;
	}

	const int eval(const Entitie& my, const EnemyReapers& enemy, const Entitie& wreck, const array<int, 3>& arrival, const array<Tankers, 10>& tankers) const {

		const int maxTurn = (int)tankers.size();

		const auto pow = [](int r) { return r*r; };

		//得られる水量
		int water = 0;
		int turn = 0;
		if (arrival[0] < maxTurn)
		{
			int harvest = 0;
			for (int t = min(arrival[0], min(arrival[1], arrival[2])); t < maxTurn; t++)
			{
				turn = t;
				bool flag = true;
				for (const auto& tanker : tankers[t])
				{
					if (range(tanker.pos, wreck.pos) < pow(tanker.radius + PlayerRadius))
					{
						flag = false;
						break;
					}
				}
				if (flag)
				{
					if (arrival[0] <= t)
					{
						water++;
						harvest++;
					}
					//if (arrival[1] <= t) { harvest++; }
					//if (arrival[2] <= t) { harvest++; }
				}
				if (harvest >= wreck.extra1)
				{
					break;
				}
			}
		}

		return water * 100 / max(turn, 1);
	}

};

class AI {
public:

	AI() {

	}

	const array<string, 3> think() {

		//scoreMap.makeWreckMap();

		array<string, 3> command;
		command.fill(wait());

		command[1] = destroyerThink();
		command[0] = reaperThink();
		command[2] = doofThink();

		return command;
	}

private:

	ReaperPos reaperPos;

	const string reaperThink() {

		string command = throttle(Point(0, 0), 1);
		bool input = false;

		const auto& reaper = Share::getMyReapers();
		const auto& destroyer = Share::getMyDestroyers();
		const auto& wrecks = Share::getfWrecks();
		const auto& tankers = Share::getfTankers();

		const auto& eReapers = Share::getEnReapers();
		const auto& eDoofs = Share::getEnDoofs();

		const auto canTar = []() {
			return Share::getMyRage() >= TarCost;
		};

		//cerr << "R:" << reaper.pos << endl;
		//cerr << "D:" << destroyer.pos << endl;

		if (canTar())
		{

		}
		if (!input)
		{
			if (!wrecks.empty())
			{
				const Point pos = reaperPos.getPos();

				if (pos.x != Inf)
				{
					//command = reaperSimulation.think(pos);
					command = speedControl(reaper.pos, pos, reaper.v, ReaperType);
					input = true;
				}
			}
		}

		if (!input)
		{
			const Point pos = destroyer.pos - Point(destroyer.v.x * 5, destroyer.v.y * 5);

			command = speedControl(reaper.pos, pos, reaper.v, ReaperType);
		}

		return command;
	}

	const string destroyerThink() {

		string command = throttle(Point(0, 0), 1);
		bool input = false;

		const auto& reaper = Share::getMyReapers();
		const auto& destroyer = Share::getMyDestroyers();
		const auto& tankers = Share::getTankers();
		const auto& wrecks = Share::getWrecks();

		const auto& enReapers = Share::getEnReapers();
		const auto& enDoofs = Share::getEnDoofs();

		const auto canGrenade = []() {
			return Share::getMyRage() >= GrenadeCost;
		};

		if (canGrenade())
		{
			if (range(reaper.pos, destroyer.pos) < range(GrenadeRange))
			{
				const bool near = [&]() {

					for (const auto& w : wrecks)
					{
						if (range(reaper.pos, w.pos) < range(2000))
						{
							return true;
						}
					}
					return false;
				}();

				if (near)
				{
					for (const auto& ed : enDoofs)
					{
						if (range(reaper.pos, ed.pos) < range(GrenadeRadius))
						{
							command = skill(reaper.pos, "!!Reaper!!");
							input = true;
							break;
						}
					}
					for (const auto& er : enReapers)
					{
						if (range(reaper.pos, er.pos) < range(GrenadeRadius))
						{
							command = skill(reaper.pos, "!!Reaper!!");
							input = true;
							break;
						}

					}
				}
			}
			else
			{
				for (const auto& w : wrecks)
				{
					if (range(destroyer.pos, w.pos) < range(GrenadeRange))
					{
						for (const auto& er : enReapers)
						{
							if (range(destroyer.pos, w.pos) < range(GrenadeRange))
							{
								if (range(w.pos, er.pos + er.v) < w.radius*w.radius)
								{
									const auto& dv = [](const Point& v) {
										const double d = atan2(v.y, v.x);
										int dx = static_cast<int>(cos(d) * 3);
										int dy = static_cast<int>(sin(d) * 3);
										return Point(dx, dy);
									}(er.v);
									command = skill(er.pos - dv, "!!Wrecks!!");
									input = true;
									break;
								}
							}
						}
					}
				}
			}

		}

		if (range(destroyer.pos, reaper.pos) >= range(6000))
		{
			command = throttle(reaper.pos, ThrottleMaxPower);
			input = true;
		}

		if (!input)
		{
			if (!tankers.empty())
			{
				int maxScore = -range(FieldRadius);
				bool find = false;
				Entitie e;

				for (const auto& t : tankers)
				{
					if (range(Point(0, 0), t.pos) <= range(FieldRadius))
					{
						const int score = t.extra2;

						if (maxScore < score)
						{
							maxScore = score;
							e = t;
							find = true;
						}
					}
				}
				if (find)
				{
					if (range(Point(0, 0), e.pos) <= range(FieldRadius))
						if (range(e.pos, destroyer.pos) < 1500)
							Share::add_wreck(e);

					const int r = (e.radius + PlayerRadius + 500);
					if (range(e.pos, destroyer.pos) < r*r)
					{
						if (range(reaper.pos, e.pos) < (e.radius + PlayerRadius + 750)*(e.radius + PlayerRadius + 750))
						{
							command = speedControl(destroyer.pos, e.pos, destroyer.v, DestroyerType);
							input = true;
						}
						else
						{
							const Point vzero = destroyer.pos - destroyer.v;
							command = speedControl(destroyer.pos, vzero, destroyer.v, DestroyerType);
							input = true;
						}
					}
					else
					{
						command = speedControl(destroyer.pos, e.pos, destroyer.v, DestroyerType);
						input = true;
					}
				}
			}
		}

		if (!input)
		{
			command = throttle(Point(0, 0), ThrottleMaxPower);
		}

		return command;
	}

	const string doofThink() {

		string command = throttle(Point(0, 0), 1);
		bool input = false;

		const auto& reaper = Share::getMyReapers();
		const auto& destroyer = Share::getMyDestroyers();
		const auto& doof = Share::getMyDoofs();

		const auto& ereaper = Share::getEnReapers();
		const auto& edestroyer = Share::getEnDestroyers();
		const auto& edoof = Share::getEnDoofs();

		const auto& tankers = Share::getTankers();
		const auto& wrecks = Share::getWrecks();

		const auto canOil = []() {
			return Share::getMyRage() >= OilCost;
		};

		static int coolTime = 0;

		if (coolTime <= 0 && canOil())
		{
			for (const auto& w : wrecks)
			{
				if (range(doof.pos, w.pos) < range(OilRange))
				{
					for (const auto& er : ereaper)
					{
						if (range(w.pos, er.pos) < w.radius*w.radius*2.25)
						{
							if (range(reaper.pos, w.pos) > w.radius*w.radius*2.25)
							{
								command = skill(w.pos, "!Oil!");
								input = true;
								Share::useRage(OilCost);
								coolTime = 2;
								break;
							}
						}
					}
				}
				if (input)
					break;
			}

		}
		coolTime--;

		if (!input)
		{
			if (Share::getMyRage() >= 200)
			{
				const int topId = Share::getTopId();
				for (const auto& er : ereaper)
				{
					if (er.player == topId)
					{
						command = speedControl(doof.pos, er.pos, doof.v, DoofType);
						input = true;
					}
				}
			}
		}

		if (!input)
		{
			command = doofSimulation.think();
		}
		return command;
	}

	DoofSimulation doofSimulation;
	ReaperSimulation reaperSimulation;

};

int main() {

	Input::first();
	AI ai;

	Stopwatch sw;

	while (true)
	{
		Input::loop();

		sw.start();
		const auto& command = ai.think();

		cout << command[0] << endl;
		cout << command[1] << endl;
		cout << command[2] << endl;
		sw.stop();

		cerr << sw.toString_us() << endl;

	}

	return 0;
}
