#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <iostream>
#include <string>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <X11/X.h>
#include <vector>
#include <xcb/xproto.h>
#include <mutex>
#include <queue>
#include <cstring> // For strerror
#include <cerrno>  // For errno

using namespace std;

class FileHandler
{
	private:
	/* Variabels   */
		string m_filename;
		
	public:
	/* Methods 	   */
		bool append(const string &text) 
		{
			ofstream file(m_filename, ios::app);// Open in append mode

			if (!file.is_open()) 
			{
				return false;// Return false if file couldn't be opened
			}
			file << text;// Append text
			file.close();
			return true;
		}

		bool open()
		{
			ofstream file(m_filename, ios::app);// Open in append mode
			bool isOpen = file.is_open();
			file.close();
			return isOpen;
		}

	/* Constructor */
		explicit FileHandler(std::string filename)
		: m_filename(std::move(filename)) {}

};

class TIME
{
	public:
		static const string get()
		{
			// Get the current time
			const auto & now = chrono::system_clock::now();
			const auto & in_time_t = chrono::system_clock::to_time_t(now);

			// Convert time_t to tm as local time
			tm buf{};
			localtime_r(&in_time_t, &buf);

			// Use stringstream to format the time
			ostringstream ss;
			ss << "[" << put_time(&buf, "%Y-%m-%d %H:%M:%S") << "]";
			return ss.str();
		}

        static string mili()
        {
            // Get the current time point
            auto now = chrono::system_clock::now();

            // Convert to time_t for seconds and tm for local time
            auto in_time_t = chrono::system_clock::to_time_t(now);
            tm buf{};
            localtime_r(&in_time_t, &buf);

            // Use stringstream to format the time
            std::ostringstream ss;
            ss << "[" << put_time(&buf, "%Y-%m-%d %H:%M:%S");

            // Calculate milliseconds (now time since epoch minus time_t converted back to time since epoch)
            auto since_epoch = now.time_since_epoch();
            auto s = chrono::duration_cast<chrono::seconds>(since_epoch);
            since_epoch -= s;
            auto ms = chrono::duration_cast<chrono::milliseconds>(since_epoch);

            // Append milliseconds to the formatted string, correctly placing the closing square bracket
            ss << "." << setfill('0') << setw(3) << ms.count() << "]";

            return ss.str();
        }
};

class Converter
{
	public:
		static const char *xcb_event_response_type_to_str(uint8_t __response_type) 
		{
			switch (__response_type) 
			{
				case XCB_KEY_PRESS:             return "KeyPress";
				case XCB_KEY_RELEASE:           return "KeyRelease";
				case XCB_BUTTON_PRESS:          return "ButtonPress";
				case XCB_BUTTON_RELEASE:        return "ButtonRelease";
				case XCB_MOTION_NOTIFY:         return "MotionNotify";
				case XCB_ENTER_NOTIFY:          return "EnterNotify";
				case XCB_LEAVE_NOTIFY:          return "LeaveNotify";
				case XCB_FOCUS_IN:              return "FocusIn";
				case XCB_FOCUS_OUT:             return "FocusOut";
				case XCB_KEYMAP_NOTIFY:         return "KeymapNotify";
				case XCB_EXPOSE:                return "Expose";
				case XCB_GRAPHICS_EXPOSURE:     return "GraphicsExpose";
				case XCB_NO_EXPOSURE:           return "NoExpose";
				case XCB_VISIBILITY_NOTIFY:     return "VisibilityNotify";
				case XCB_CREATE_NOTIFY:         return "CreateNotify";
				case XCB_DESTROY_NOTIFY:        return "DestroyNotify";
				case XCB_UNMAP_NOTIFY:          return "UnmapNotify";
				case XCB_MAP_NOTIFY:            return "MapNotify";
				case XCB_MAP_REQUEST:           return "MapRequest";
				case XCB_REPARENT_NOTIFY:       return "ReparentNotify";
				case XCB_CONFIGURE_NOTIFY:      return "ConfigureNotify";
				case XCB_CONFIGURE_REQUEST:     return "ConfigureRequest";
				case XCB_GRAVITY_NOTIFY:        return "GravityNotify";
				case XCB_RESIZE_REQUEST:        return "ResizeRequest";
				case XCB_CIRCULATE_NOTIFY:      return "CirculateNotify";
				case XCB_CIRCULATE_REQUEST:     return "CirculateRequest";
				case XCB_PROPERTY_NOTIFY:       return "PropertyNotify";
				case XCB_SELECTION_CLEAR:       return "SelectionClear";
				case XCB_SELECTION_REQUEST:     return "SelectionRequest";
				case XCB_SELECTION_NOTIFY:      return "SelectionNotify";
				case XCB_COLORMAP_NOTIFY:       return "ColormapNotify";
				case XCB_CLIENT_MESSAGE:        return "ClientMessage";
				case XCB_MAPPING_NOTIFY:        return "MappingNotify";			
				case XCB_GE_GENERIC:			return "GeneriqEvent";						            
				default:                        return "Unknown";
			}    
		}
		
		static const char *xcb_event_detail_to_str(uint8_t __detail) 
		{
			switch (__detail)
			{
				case XCB_NOTIFY_DETAIL_ANCESTOR: 			return "Ancestor";
				case XCB_NOTIFY_DETAIL_VIRTUAL: 			return "Virtual";
				case XCB_NOTIFY_DETAIL_INFERIOR: 			return "Inferior";
				case XCB_NOTIFY_DETAIL_NONLINEAR: 			return "Nonlinear";
				case XCB_NOTIFY_DETAIL_NONLINEAR_VIRTUAL: 	return "NonlinearVirtual";
				case XCB_NOTIFY_DETAIL_POINTER: 			return "Pointer";
				case XCB_NOTIFY_DETAIL_POINTER_ROOT: 		return "PointerRoot";
				case XCB_NOTIFY_DETAIL_NONE: 				return "None";
				default: 									return "Unknown";
			}
		}

		static const char *xcb_event_mode_to_str(uint8_t __mode)
		{
			switch (__mode) 
			{
				case XCB_NOTIFY_MODE_NORMAL: 		return "Normal";
				case XCB_NOTIFY_MODE_GRAB: 			return "Grab";
				case XCB_NOTIFY_MODE_UNGRAB: 		return "Ungrab";
				case XCB_NOTIFY_MODE_WHILE_GRABBED: return "WhileGrabbed";
				default: 							return "Unknown";
			}
		}
};

class Log
{
	public:
		static void xcb_event_response_type(const char* FUNC, const uint8_t &response_type) 
		{
			const char* ev = Converter::xcb_event_response_type_to_str(response_type);
			logMessage
			( 
				toString
				(
					":[xcb_event_response_type]:[", FUNC,"]:[",ev,"]"
				)
			);
		}

		static void xcb_event_detail(const char* FUNC, const uint8_t &detail) 
		{
			const char* ev = Converter::xcb_event_detail_to_str(detail);
			logMessage 
			( 
				toString
				(
					":[xcb_event_detail]:[", FUNC, "]:[", ev, "]"
				)
			);
		}
		
		static void FUNC(const string &message)
		{
			logMessage(":[" + message + "]");
		}
		
		static void Start(const string &message)
		{
			StartMessage( ":[Start]  :["+message+"]:[STARTED]" );
		}
		
		static void End() 
		{
			EndMessage( ":[End]    :[dwm-M]:[KILLED]\n" );
		}
		
		static void ENDFUNC(const string &message)
		{
			logMessage ( ":[ENDFUNC]:[" + message + "]" + "\n");
		}

		static void INFO(const string &message)
		{
			logMessage(":[INFO]:[" + message + "]");
		}

		template<typename T, typename... Args>
		static void INFO(const T &message, Args... args) 
		{
			logMessage ( ":[INFO]   :[" + toString ( message, args... ) + "]" );
		}

		template<typename T, typename... Args>
		static void FUNC_INFO(const T &message, Args... args)
		{
			logMessage(":----[INFO]:[" + toString(message, args...) + "]");
		}

		static void WARNING(const string &message)
		{
			logMessage ( ":[WARNING]:[" + message + "]" );
		}

		template<typename T, typename... Args>
		static void WARNING(const T &first, Args... args)
		{
			logMessage ( ":[WARNING]:[" + toString ( first, args... ) + "]" );
		}

		static void ERROR(const string &message)
		{
			logMessage ( ":[ERROR]  :[" + message + "]" );
		}

		template<typename T, typename... Args>
		static void ERROR(const T &message, Args... args)
		{
			logMessage(":[ERROR]  :[" + toString(message, args...) + "]");
		}

	private:
		// Static function for conversion
		template<typename T>
		static string toString(const T &arg) 
		{
			ostringstream stream;
			stream << arg;
			return stream.str();
		}

		template<typename T, typename... Args>
		static string toString(const T &message, Args... args) 
		{
			return toString(message) + toString(args...);
		}

		static void logMessage(const std::string &message) 
		{
			FileHandler file ( "/home/mellw/nlog" );

			if (file.open())
			{
				file.append(TIME::get() + message + "\n");
			}
		}

		static void StartMessage(const string &message) 
		{
			FileHandler file ("/home/mellw/nlog");

			if (file.open()) 
			{
				file.append("\n" + TIME::get() + message + "\n");
			}
		}

		static void EndMessage(const string &message) 
		{
			FileHandler file("/home/mellw/nlog");

			if (file.open()) 
			{
				file.append(TIME::get() + message + "\n");
			}
		}
};

class toString 
{
	public:
		template<typename T>
		toString(const T &input)
		{
			result = convert(input);
		}

		operator string() const 
		{
			return result;
		}

	private:
		string result;

		string convert(const int16_t &in)
		{
			string str;
			str = to_string(in);
			return str;
		}

		string convert(const string &in)
		{
			return in;
		}

		string convert(const vector<uint32_t> &in)
		{
			string str;
			for (auto &i : in) 
			{
				str += to_string(i) + " ";
			}
			return str;
		}

		string convert(const vector<const char *> &in)
		{
			string str;
			for (auto &i : in) 
			{
				str += string(i) + " ";
			}
			return str;
		}

		string convert(const vector<string> &in)
		{
			string str;
			for (auto &i : in) 
			{
				str += i + " ";
			}
			return str;
		}

		string convert(const std::vector<xcb_event_mask_t> &in)
		{
			string str;
			for (auto &i : in) 
			{
				str += to_string(i) + " ";
			}
			return str;
		}
};

// ANSI escape codes for colors
#define log_RED 		"\033[1;31m" 
#define log_GREEN 		"\033[1;32m" 
#define log_YELLOW 		"\033[1;33m"
#define log_BLUE 		"\033[1;34m"
#define log_MEGENTA 	"\033[1;35m"
#define log_CYAN 		"\033[1;36m"
#define log_WHITE 		"\033[1;37m"
#define log_BOLD 		"\033[1m"
#define log_UNDERLINE 	"\033[4m"
#define log_RESET 		"\033[0m"


enum LogLevel
{
    INFO,
	INFO_PRIORITY,
    WARNING,
    ERROR,
	FUNC
};

class Logger
{
	public:
		template<typename T, typename... Args>
		void log(LogLevel level, const std::string &function, const T& message, Args&&... args) 
		{
			FileHandler file("/home/mellw/nlog");
			log_arragnge(list, message, args...);
			file.append(TIME::get() + ":" + getLogPrefix(level) + ":" + log_MEGENTA + "[" + function + "]" + log_RESET + ":" + str + "\n");
		}

		void log(LogLevel level, const std::string &function) 
		{
			FileHandler file("/home/mellw/nlog");
			file.append(TIME::get() + ":" + getLogPrefix(level) + ":[" + log_MEGENTA + function + log_RESET + "]\n");
		}
	;

	private:
		vector<string> list;
		string str;

		string getLogPrefix(LogLevel level) const 
		{
			switch (level) 
			{
				case INFO:
				{
					return log_GREEN 	"[INFO]" 		  log_RESET;
				}
				
				case INFO_PRIORITY:
				{
					return log_CYAN		"[INFO_PRIORITY]" log_RESET;
				}
				
				case WARNING:
				{
					return log_YELLOW 	"[WARNING]" 	  log_RESET;
				}
				
				case ERROR:
				{
					return log_RED 		"[ERROR]" 		  log_RESET;
				}
				
				case FUNC:
				{
					return log_MEGENTA	"[FUNC]"		  log_RESET;
				}
				
				default:
				{
					return to_string(level);
				}
			}
		}

		template<typename T, typename... Args>
		void log_arragnge(vector<string> list, const T &message, const Args&... args)
		{
			list.push_back(toString(message));
			if constexpr (sizeof...(args) > 0)
			{
				log_arragnge(list, args...);
			}
			else
			{
				log_append(list);
			}
		}

		void log_append(vector<string> list)
		{
			string result;
			int current = 0;
			for (auto &i : list)
			{
				result += "[" + i + "]";
				if (current != list.size() - 1) 
				{
					result += ":";
				}
				current++;
			}
			str = result;
		}
	;
};

typedef struct
{
	string value;
}
event_type_obj_t;

inline constexpr const char *xcb_event_type_to_str(uint8_t __event_type)
{
	switch (__event_type)
	{
		case  2: return "XCB_KEY_PRESS        ";
		case  3: return "XCB_KEY_RELEASE      ";
		case  4: return "XCB_BUTTON_PRESS     ";
		case  5: return "XCB_BUTTON_RELEASE   ";
		case  6: return "XCB_MOTION_NOTIFY    ";
		case  7: return "XCB_ENTER_NOTIFY     ";
		case  8: return "XCB_LEAVE_NOTIFY     ";
		case  9: return "XCB_FOCUS_IN         ";
		case 10: return "XCB_FOCUS_OUT        ";
		case 11: return "XCB_KEYMAP_NOTIFY    ";
		case 12: return "XCB_EXPOSE           ";
		case 13: return "XCB_GRAPHICS_EXPOSURE";
		case 14: return "XCB_NO_EXPOSURE      ";
		case 15: return "XCB_VISIBILITY_NOTIFY";
		case 16: return "XCB_CREATE_NOTIFY    ";
		case 17: return "XCB_DESTROY_NOTIFY   ";
		case 18: return "XCB_UNMAP_NOTIFY     ";
		case 19: return "XCB_MAP_NOTIFY       ";
		case 20: return "XCB_MAP_REQUEST      ";
		case 21: return "XCB_REPARENT_NOTIFY  ";
		case 22: return "XCB_CONFIGURE_NOTIFY ";
		case 23: return "XCB_CONFIGURE_REQUEST";
		case 24: return "XCB_GRAVITY_NOTIFY   ";
		case 25: return "XCB_RESIZE_REQUEST   ";
		case 26: return "XCB_CIRCULATE_NOTIFY ";
		case 27: return "XCB_CIRCULATE_REQUEST";
		case 28: return "XCB_PROPERTY_NOTIFY  ";
		case 29: return "XCB_SELECTION_CLEAR  ";
		case 30: return "XCB_SELECTION_REQUEST";
		case 31: return "XCB_SELECTION_NOTIFY ";
		case 32: return "XCB_COLORMAP_NOTIFY  ";
		case 33: return "XCB_CLIENT_MESSAGE   ";
		case 34: return "XCB_MAPPING_NOTIFY   ";
		case 35: return "XCB_GE_GENERIC       ";

		default: return "UNKNOWN";
	}
}

typedef struct
{
    string value;
}
FuncNameWrapper;

typedef struct
{
	string value;
}
file_name_obj_t;

typedef struct
{
	int line;
}
line_obj_t;

typedef struct
{
    LogLevel level;
    string function;
    int line;
    string message;
    // Include a timestamp if you prefer logging it to be handled by the logger rather than each log call
}
LogMessage;

typedef struct
{
    uint32_t value;
}
window_obj_t;

typedef struct
{
	string value;
}
errno_msg_t;

class LogQueue
{
	public:
		void push(const LogMessage& message)
		{
			lock_guard<mutex> guard(mutex_);
			queue_.push(message);
		}

		bool try_pop(LogMessage& message)
		{
			lock_guard<mutex> guard(mutex_);
			if (queue_.empty())
			{
				return false;
			}

			message = queue_.front();
			queue_.pop();
			return true;
	    }

	private:
		mutex mutex_;
		queue<LogMessage> queue_;
};

class lout
{
    /* Defines 	 */
        #define loutNUM(__variable) \
            "(\033[33m" << __variable << "\033[0m)"

		#define loutCFUNC(__calling_function) \
			"calling_function(\033[35m" << __calling_function << "\033[0m)"

		#define lout_error_code(__error_code) \
			"\033[31merror_code\033[0m" << __error_code

		#define loutCEcode(__error_code) \
			lout_error_code(__error_code) << " " << loutCFUNC(__calling_function)

		#define loutPath(__path) \
			"(\033[32m" << __path << "\033[0m)"

		#define loutUser(__user) \
			"(" << log_BLUE << __user << log_RESET << ")"

		#define loutEND \
			'\n'

		#define Var_(_Var) #_Var << ' ' << _Var

	public:
	/* Methods 	 */
		/**
		 *
		 * @brief Make lout a singleton to ensure a single object instance
		 *
		 * NOTE: not used for now
		 */
		 // static lout& inst()
		 // {
		 // 	static lout instance;
		 // 	return instance;
		 // }

        lout() {}
		
		lout& operator<<(LogLevel logLevel) // Overloads for handling log level, function name wrapper, and endl
		{
			currentLevel = logLevel;
			return *this;
		}

		lout& operator<<(const event_type_obj_t &__event_type)
		{
			buffer << "event_type" << '(' << log_BLUE << __event_type.value << log_RESET << ')';
			return *this;
		}

		lout& operator<<(const FuncNameWrapper& funcName)
		{
			currentFunction = funcName.value;
			return *this;
		}

		lout& operator<<(const line_obj_t &__line)
		{
			current_line = __line.line;
			return *this;
		}

		/**
		 *
		 * TODO: Add this into the logMessage function to append it to the log message
		 *
		 */
		lout& operator<<(const file_name_obj_t &__name)
		{
			current_file = __name.value;
			return *this;
		}

        lout& operator<<(const window_obj_t &__window)
        {
            buffer << "[" << log_BLUE << "WINDOW_ID" << log_RESET << ":" << loutNUM(__window.value) << "] ";
            return *this;
        }

		lout& operator<<(ostream& (*pf)(ostream&))
		{
			if (pf == static_cast<std::ostream& (*)(std::ostream&)>(std::endl))
			{
				logMessage();
				buffer = std::ostringstream(); // Reset the buffer for new messages
				// Reset log level and function as well if desired
			}
			return *this;
		}

        lout& operator<<(char c)
		{
        	if (c == '\n') {
                logMessage();
				buffer = std::ostringstream(); // Reset the buffer for new messages
			} else {
                buffer << c;
            }
			return *this;
        }

		lout& operator<<(const errno_msg_t &__errno)
		{
			buffer << __errno.value;
			return *this;
		}
		
        template<typename T>
        typename enable_if<is_arithmetic<T>::value, lout&>::type
        operator<<(T value)
		{
            buffer << loutNUM(value);
            return *this;
        }

        template<typename T>
        typename enable_if<!is_arithmetic<T>::value, lout&>::type
        operator<<(const T& message)
		{
            buffer << message;
            return *this;
        }

		/* lout& err(const string &__err_msg)
		{
			buffer << __err_msg << ": " << strerror(errno) << " (errno: " << errno << ")";
			return *this;
		} */

		/* template<typename T>
		lout& operator<<(const T& message)
		{
			buffer << message;
			return *this;
		} */

	private:
	/* Variabels */
		LogLevel currentLevel;
		string currentFunction;
		string current_file;
		int current_line;
		ostringstream buffer;
		mutex log_mutex;

	/* Methods 	 */
		void logMessage()
		{
			lock_guard<mutex> guard(log_mutex);

			ofstream file("/home/mellw/nlog", ios::app); // Append mode
			if (file)
			{
				file << TIME::mili() << ":" << getLogPrefix(currentLevel) << ":" << log_YELLOW << "[Line:" << current_line << "]" << log_RESET << ":" << log_MEGENTA << "[" << currentFunction << "]" << log_RESET << ": " << buffer.str() << "\n";
			}
		}

		string getLogPrefix(LogLevel level) const 
		{
			switch (level) 
			{
				case INFO:
				{
					return log_GREEN 	"[INFO]" 		  log_RESET;
				}
				case INFO_PRIORITY:
				{
					return log_CYAN		"[INFO_PRIORITY]" log_RESET;
				}
				case WARNING:
				{
					return log_YELLOW 	"[WARNING]" 	  log_RESET;
				}
				case ERROR:
				{
					return log_RED 		"[ERROR]" 		  log_RESET;
				}
				case FUNC:
				{
					return log_MEGENTA	"[FUNC]"		  log_RESET;
				}
				default:
				{
					return to_string(level);
				}
			}
		}
};
static class lout lout;

// Utility function for wrapping function names
inline FuncNameWrapper func(const char* name)
{
    return FuncNameWrapper{name};
}

inline file_name_obj_t file_name(const char* name)
{
	return file_name_obj_t{name};
}

inline line_obj_t line(int __line)
{
	return line_obj_t{__line};
}

inline window_obj_t window_id(uint32_t __window)
{
    return window_obj_t{__window};
}

inline errno_msg_t errno_msg(const char *__str)
{
	string s = string(__str) + ": " + strerror(errno) + " (errno: " + to_string(errno) + ")";
	return {s};
}

inline event_type_obj_t event_type(uint8_t __event_type)
{
	return {xcb_event_type_to_str(__event_type)};
}

#define FUNC func(__func__)
#define LINE line(__LINE__)
#define FILE_NAME file_name(__FILE__)
#define WINDOW_ID window_id(_window)
#define ERRNO_MSG(__msg) errno_msg(__msg)
#define EVENT_TYPE(__event_type) event_type(__event_type)

#define WINDOW_ID_BY_INPUT(__window) \
	window_id(__window)

/* LOG DEFENITIONS */
#define loutWin(__window) \
	lout << INFO << LINE << FUNC << WINDOW_ID_BY_INPUT(__window) << loutEND 

/**
 * @brief Macro to log info to the log file
 *        using the lout class  
 */
#define loutI \
    lout << INFO << FUNC << LINE

/**
 *
 * @brief Used to log info related to window actions
 *
 * NOTE: ONLY USE THIS INSIDE THE 'window' CLASS AS VAR '_window' IS A MEMBER
 *
 */
#define loutIWin \
    loutI << WINDOW_ID

/**
 * @brief Macro to log an error to the log file
 *        using the lout class  
 */
#define loutE \
	lout << ERROR << FUNC << LINE

#define loutErrno(__msg) \
	loutE << ERRNO_MSG(__msg) << '\n'

/**
 *
 * @brief Used to log errors related to window actions
 *
 * NOTE: ONLY USE THIS INSIDE THE 'window' CLASS AS VAR '_window' IS A MEMBER
 *
 */
#define loutEWin \
    loutE << WINDOW_ID

#define loutEerror_code(__calling_function, __error_code) \
	loutE << loutCFUNC(__calling_function) << " " << lout_error_code(__error_code)

/**
 * @brief Macro to log a warning to the log file
 *        using the lout class
 */
#define loutW \
    lout << WARNING << FUNC << LINE

/**
 *
 * @brief Used to log warnings related to window actions
 *
 * NOTE: ONLY USE THIS INSIDE THE 'window' CLASS AS VAR '_window' IS A MEMBER
 *
 */
#define loutWWin \
    loutW << WINDOW_ID

/**
 * @brief Macro to log priority info to the log file
 *        using the lout class  
 */
#define loutIP \
    lout << INFO_PRIORITY << FUNC << LINE

#define loutCUser(__user) \
	loutI << "Current USER" << loutUser(__user) << '\n'

#define LOG_ev_response_type(ev)    	    Log::xcb_event_response_type(__func__, ev);
#define LOG_func                    	    Log::FUNC(__func__);
#define LOG_error(message)          	    Log::ERROR(__FUNCTION__, "]:[", message);
#define LOG_info(message)           	    Log::INFO(__FUNCTION__, "]:[", message);
#define LOG_f_info(message)         	    Log::FUNC_INFO(__FUNCTION__, "]:[", message);
#define LOG_warning(message)        	    Log::WARNING(__FUNCTION__, "]:[", message);
#define LOG_start()                 	    Log::Start("mwm");
#define log_event_response_typ()		    Log::xcb_event_response_type(__func__, e->response_type)
#define log_info(message)                   logger.log(INFO, __func__, message)
#define log_error(message)                  logger.log(ERROR, __FUNCTION__, message)
#define log_error_long(message, message_2)                  log.log(ERROR, __FUNCTION__, message, message_2)
#define log_error_code(message, err_code)   logger.log(ERROR, __FUNCTION__, message, err_code)
#define log_win(win_name ,window)           logger.log(INFO, __func__, win_name + std::to_string(window))
#define log_func                            logger.log(FUNC, __func__)
