
#include "FILES.h"
#include "UIBOXES.h"

void add_file_pixels(FILE_TYPE type, std::filesystem::path path, std::string filename, uint16_t canvas_w, uint16_t canvas_h)
{
	std::shared_ptr<FILE_INFO_PIXELS> new_file = std::make_shared<FILE_INFO_PIXELS>(
		path,
		filename,
		canvas_w,
		canvas_h);

	CURRENT_FILE = OPEN_FILES.size();
	CURRENT_FILE_PTR_PIXELS = new_file;
	CURRENT_FILE_PTR_TYPE = type;

	UIBOX_PTR_FILE_EXPLORER->update = true;
	UIBOX_PTR_OPENED_FILES->update = true;

	OPEN_FILES.push_back(std::move(new_file));
}

void open_file(std::filesystem::path path)
{
	MOUSEBUTTON_LEFT = 0;
	MOUSEBUTTON_PRESSED_LEFT = 0;

	if (path != "" && path.parent_path() != "") CURRENT_PATH = path.parent_path();
	UIBOX_PTR_FILE_EXPLORER->update = true;
	UIBOX_PTR_OPENED_FILES->update = true;
	((UIBOX_INFO_OPENED_FILES*)UIBOX_PTR_OPENED_FILES)->_open_file = path;

	for (int i = 0; i < OPEN_FILES.size(); ++i)
	{
		auto _f = OPEN_FILES[i];
		
		if (_f->path == path.string())
		{
			CURRENT_FILE = i;
			CURRENT_FILE_PTR_TYPE = _f->type;
			
			switch (CURRENT_FILE_PTR_TYPE)
			{
			case EMPTY:
				break;
			case PIXELS:
				CURRENT_FILE_PTR_PIXELS = std::static_pointer_cast<FILE_INFO_PIXELS>(OPEN_FILES[i]);
				break;
			case MAP:
				break;
			case TEXT:
				break;
			default:
				break;
			}

			return;
		}
	}

	PRINT("LOADING IMAGE...");
	auto from = std::chrono::high_resolution_clock::now();

	SDL_Surface* _surfload;
	_surfload = IMG_Load(path.string().c_str());
	if (!_surfload)
	{
		PRINT("CANNOT LOAD FILE");
		return;
	}

	SDL_Surface* _surf = SDL_ConvertSurfaceFormat(_surfload, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(_surfload);

	add_file_pixels(FILE_TYPE::PIXELS, path, path.filename().string(), _surf->w, _surf->h);
	CURRENT_FILE_PTR_PIXELS->current_layer_ptr->texture = SDL_CreateTextureFromSurface(RENDERER_MAIN, _surf);
		
	const auto size = (CURRENT_FILE_PTR_PIXELS->canvas_w * CURRENT_FILE_PTR_PIXELS->canvas_h);
	const COLOR* src = static_cast<const COLOR*>(_surf->pixels);

	auto &_pd = (CURRENT_FILE_PTR_PIXELS->current_layer_ptr->pixels);

	std::copy(src, src + size, _pd.get());

	/*auto _ps = ((COLOR*)_surf->pixels);
	auto &_pd = (CURRENT_FILE_PTR_PIXELS->current_layer_ptr->pixels);
	auto _ts = (CURRENT_FILE_PTR_PIXELS->canvas_w * CURRENT_FILE_PTR_PIXELS->canvas_h);
	for (uint32_t i = 0; i < _ts; i++)
	{
		_pd[i] = _ps[i];
	}*/

	auto to = std::chrono::high_resolution_clock::now();
	std::cout << "LOADED IN: " << std::chrono::duration_cast<std::chrono::duration<double>>(to - from).count() << " seconds" << std::endl << std::endl;
		
	SDL_FreeSurface(_surf);
}

void UPDATE_PATH_FILES()
{
	/*for (auto& _element : CURRENT_PATH)
	{

	}*/
	int _folders = 0;
	std::string _filename;
	CURRENT_PATH_FILES.clear();

	//try {
		for (auto& _element : std::filesystem::directory_iterator(CURRENT_PATH, std::filesystem::directory_options::skip_permission_denied))
		{
			_filename = _element.path().filename().string();
			//_element.status()

			if (_filename[0] != '.')
			{
				PATH_FILES_INFO _tpfi;
				_tpfi.ext = _element.path().extension().string();
				_tpfi.filename = _filename;
				_tpfi.is_directory = (_element.is_directory());
				_tpfi.path = _element.path().string();

				if (_tpfi.is_directory)
				{
					CURRENT_PATH_FILES.insert(CURRENT_PATH_FILES.begin() + _folders, std::move(_tpfi));
					_folders += 1;
				}
				else
				{
					if (_tpfi.ext == ".png" || _tpfi.ext == ".PNG") CURRENT_PATH_FILES.push_back(std::move(_tpfi));
				}
			}
		}
	//}
	//catch (std::filesystem::filesystem_error& err) {
	//	std::wcerr << L"Error: " << err.what() << std::endl;
	//}
}

std::vector<std::shared_ptr<FILE_INFO>> OPEN_FILES;

int CURRENT_FILE = -1;
FILE_TYPE CURRENT_FILE_PTR_TYPE = FILE_TYPE::EMPTY;
std::shared_ptr<FILE_INFO_PIXELS> CURRENT_FILE_PTR_PIXELS = nullptr;



