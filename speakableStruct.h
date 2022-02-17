#ifndef _SPEAK_STRUCT_
#define _SPEAK_STRUCT_

namespace hui{
	static int count = 0;
	struct Speakable{
		char* buffer = nullptr;
		Speakable(){
			buffer = new char[100];
			count++;
			std::cout << "\nSpeakable()\n";
		}
		
		~Speakable(){
			delete[] buffer;
			count--;
			std::cout << "\n~Speakable()\n";
		}

		Speakable(const Speakable& sp){
			std::cout << "\nSpeakable(const Speakable&)\n";
		}

		Speakable(Speakable&& sp){
			std::cout << "\nSpeakable(Speakable&&)\n";
		}

		Speakable& operator=(const Speakable&){
			std::cout << "\nSpeakable=(&)\n";
			return *this;
		}

		Speakable& operator=(Speakable&&){
			std::cout << "\nSpeakable=(&&)\n";
			return *this;
		}


	};

};

#endif

